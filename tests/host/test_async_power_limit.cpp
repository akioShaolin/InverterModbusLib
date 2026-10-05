#include "Inverter.h"
#include <cstdlib>
#include <limits>

HardwareSerial Serial;
uint32_t hostMillis = 0;
static ModbusInverterMap customMap;
static InverterDescriptor customDescriptor;

// The two actual field models use the production maps and descriptors. UNKNOWN
// is an isolated malformed-map fixture for validating numeric boundaries.
const InverterDescriptor getDescriptor(InverterModel model) {
    return model == UNKNOWN_INVERTER ? customDescriptor : getDescriptor_Weg(model);
}
bool getInverterMap(InverterModel model, ModbusInverterMap& out) {
    if (model == UNKNOWN_INVERTER) { out = customMap; return true; }
    return getMap_Weg(model, out);
}

#define CHECK(condition) do { if (!(condition)) { \
    std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #condition); std::abort(); \
} } while (false)

struct Fixture {
    ModbusRTU mb;
    InverterModbusBus bus;
    Inverter inverter;
    explicit Fixture(InverterModel model) : bus(mb), inverter(model) {
        CHECK(bus.begin(Serial, 9600, SERIAL_8N1, 12));
        inverter.attachBus(bus);
        inverter.setSlaveId(1);
        CHECK(inverter.begin());
    }
    void reply(Modbus::ResultCode result = Modbus::EX_SUCCESS,
               const std::vector<uint16_t>& values = {}) {
        hostMillis += 25;
        mb.respond(result, values);
        bus.task();
    }
};

static void rejectedRequestMustNotKeepItsOldValue() {
    Fixture f(SIW500H_ST030_M3);
    float frequency = 0;
    CHECK(f.inverter.getGridFrequency(frequency) == INV_BUSY);
    CHECK(f.inverter.setPowerLimitPercent(10) == INV_REJECTED);
    f.reply(Modbus::EX_SUCCESS, {6000});
    CHECK(f.inverter.getGridFrequency(frequency) == INV_DONE);
    CHECK(f.inverter.setPowerLimitPercent(90) == INV_BUSY);
    CHECK(f.mb.sent.back().address == 0x9CBD);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({900}));
    f.reply();
    CHECK(f.inverter.setPowerLimitPercent(90) == INV_DONE);

    CHECK(f.inverter.getGridFrequency(frequency) == INV_BUSY);
    CHECK(f.inverter.setPowerLimitPercent(10) == INV_REJECTED);
    f.reply(Modbus::EX_SUCCESS, {6000});
    CHECK(f.inverter.getGridFrequency(frequency) == INV_DONE);
    CHECK(f.inverter.setPowerLimit(27000) == INV_BUSY);
    CHECK(f.mb.sent.back().function == 16);
    CHECK(f.mb.sent.back().address == 0x9CBE);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({0, 27000}));
    f.reply();
    CHECK(f.inverter.setPowerLimit(27000) == INV_DONE);
    CHECK(!f.bus.isBusy());
}

static void busOwnershipAndRequestIdentityArePreserved() {
    Fixture f(SIW500H_ST030_M3);
    Inverter second(SIW500H_ST030_M3);
    second.attachBus(f.bus);
    second.setSlaveId(2);
    CHECK(second.begin());
    CHECK(f.inverter.setPowerLimitPercent(90) == INV_BUSY);
    CHECK(f.inverter.setPowerLimit(15000) == INV_REJECTED);
    CHECK(second.setPowerLimitPercent(80) == INV_REJECTED);
    f.reply();
    CHECK(f.bus.isBusy()); // Completed result still belongs to its original caller.
    CHECK(second.setPowerLimitPercent(80) == INV_REJECTED);
    CHECK(f.inverter.setPowerLimitPercent(90) == INV_DONE);
    CHECK(second.setPowerLimitPercent(70) == INV_BUSY);
    CHECK(f.mb.sent.back().id == 2);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({700}));
    f.reply();
    CHECK(second.setPowerLimitPercent(70) == INV_DONE);
}

static void goodweEnableAndFallbackUseTheMeasuredRatedPower() {
    Fixture f(SIW400G_T100_W0);
    float rated = 0;
    CHECK(f.inverter.getRatedPower(rated) == INV_BUSY);
    f.reply(Modbus::EX_SUCCESS, {900}); // Real map: scale 100, hence 90 kW.
    CHECK(f.inverter.getRatedPower(rated) == INV_DONE);
    CHECK(rated == 90000);
    CHECK(!f.inverter.wasLastRatedPowerFallback());
    CHECK(f.inverter.setPowerLimit(81000) == INV_BUSY);
    CHECK(f.mb.sent.back().function == 6);
    CHECK(f.mb.sent.back().address == 0x9D6B);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({1}));
    f.reply();
    CHECK(f.inverter.setPowerLimit(81000) == INV_BUSY);
    CHECK(f.mb.sent.back().address == 0x9D6C);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({900}));
    f.reply();
    CHECK(f.inverter.setPowerLimit(81000) == INV_DONE);
    CHECK(f.inverter.getLastModbusStatus() == INV_MB_SUCCESS);
    CHECK(f.inverter.setPowerLimitPercent(101) == INV_ERROR);
    CHECK(f.inverter.getLastModbusStatus() == INV_MB_NONE);
}

static void timeoutAndStartFailureDoNotLeakThePreparedCommand() {
    Fixture f(SIW400G_T100_W0);
    CHECK(f.inverter.setPowerLimitPercent(90) == INV_BUSY);
    f.reply(Modbus::EX_TIMEOUT);
    CHECK(f.inverter.setPowerLimitPercent(90) == INV_ERROR);
    CHECK(f.inverter.getLastModbusStatus() == INV_MB_TIMEOUT);
    CHECK(f.mb.sent.size() == 1); // Failed enable must never send the limit.
    CHECK(!f.bus.isBusy());

    CHECK(f.inverter.setPowerLimitPercent(100) == INV_BUSY);
    CHECK(f.inverter.getLastModbusStatus() == INV_MB_NONE);
    f.reply();
    f.mb.failNextStart = true;
    CHECK(f.inverter.setPowerLimitPercent(100) == INV_ERROR);
    CHECK(f.inverter.getLastModbusStatus() == INV_MB_GENERAL_FAILURE);
    CHECK(!f.bus.isBusy());
    CHECK(f.inverter.setPowerLimitPercent(80) == INV_BUSY);
    f.reply();
    CHECK(f.inverter.setPowerLimitPercent(80) == INV_BUSY);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({800}));
    f.reply();
    CHECK(f.inverter.setPowerLimitPercent(80) == INV_DONE);
}

static void invalidInputsNeverReachModbus() {
    Fixture f(SIW500H_ST030_M3);
    const float invalid[] = {std::numeric_limits<float>::quiet_NaN(),
                            std::numeric_limits<float>::infinity(),
                            -std::numeric_limits<float>::infinity(), -1};
    for (float value : invalid) {
        CHECK(f.inverter.setPowerLimit(value) == INV_ERROR);
        CHECK(f.inverter.setPowerLimitPercent(value) == INV_ERROR);
        CHECK(f.inverter.getLastModbusStatus() == INV_MB_NONE);
        CHECK(!f.bus.isBusy());
    }
    CHECK(f.inverter.setPowerLimit(30001) == INV_ERROR);
    CHECK(f.inverter.setPowerLimitPercent(100.01f) == INV_ERROR);
    CHECK(f.mb.sent.empty());
    CHECK(f.inverter.setPowerLimitPercent(100) == INV_BUSY);
    CHECK(f.mb.sent.back().values == std::vector<uint16_t>({1000}));
    f.reply();
    CHECK(f.inverter.setPowerLimitPercent(100) == INV_DONE);
}

static void numericEncodingBoundariesAreCheckedBeforeWriting() {
    CHECK(getMap_Weg(SIW500H_ST030_M3, customMap));
    customDescriptor = getDescriptor_Weg(SIW500H_ST030_M3);
    customDescriptor.ratedPowerW = UINT32_MAX;
    {
        Fixture f(UNKNOWN_INVERTER);
        // float(UINT32_MAX) equals 2^32: casting it to uint32_t is out of range.
        CHECK(f.inverter.setPowerLimit((float)UINT32_MAX) == INV_ERROR);
        CHECK(f.mb.sent.empty());
    }
    customMap.activePower.percent.scale = std::numeric_limits<float>::quiet_NaN();
    {
        Fixture f(UNKNOWN_INVERTER);
        CHECK(f.inverter.setPowerLimitPercent(90) == INV_ERROR);
        CHECK(f.mb.sent.empty());
    }
}

int main() {
    rejectedRequestMustNotKeepItsOldValue();
    busOwnershipAndRequestIdentityArePreserved();
    goodweEnableAndFallbackUseTheMeasuredRatedPower();
    timeoutAndStartFailureDoNotLeakThePreparedCommand();
    invalidInputsNeverReachModbus();
    numericEncodingBoundariesAreCheckedBeforeWriting();
    std::puts("PASS: 6 async power-limit regression scenarios (host fakes, no hardware).");
}
