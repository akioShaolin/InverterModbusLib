#pragma once

const char rootPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <title>InverterModbusLib - Web Panel</title>
    <style>
        body {
            font-family: Arial, Helvetica, sans-serif;
            background: #f4f6f8;
            margin: 0;
            color: #222;
        }

        header {
            background: #0b3d5c;
            color: white;
            padding: 16px 20px;
        }

        header h1 {
            font-size: 22px;
            margin: 0 0 10px 0;
        }

        nav a {
            display: inline-block;
            color: white;
            text-decoration: none;
            background: #1565c0;
            padding: 8px 12px;
            border-radius: 8px;
            margin-right: 8px;
        }

        .wrap {
            max-width: 1180px;
            margin: 0 auto;
            padding: 18px;
        }

        .status {
            background: #fff;
            border-left: 5px solid #1565c0;
            padding: 12px;
            border-radius: 10px;
            margin-bottom: 16px;
            box-shadow: 0 1px 5px rgba(0, 0, 0, .08);
        }

        .grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 14px;
        }

        .card {
            background: white;
            border-radius: 12px;
            padding: 14px;
            box-shadow: 0 2px 8px rgba(0, 0, 0, .12);
        }

        .card h2 {
            font-size: 17px;
            margin: 0 0 10px 0;
            color: #0b3d5c;
        }

        form {
            margin: 0 0 10px 0;
        }

        label {
            display: block;
            font-size: 13px;
            font-weight: bold;
            margin-top: 8px;
            margin-bottom: 4px;
        }

        input,
        select {
            width: 100%;
            box-sizing: border-box;
            padding: 9px;
            border: 1px solid #bbb;
            border-radius: 8px;
            font-size: 15px;
            background: white;
        }

        button,
        .btn {
            display: inline-block;
            width: 100%;
            box-sizing: border-box;
            border: 0;
            border-radius: 8px;
            background: #1565c0;
            color: white;
            padding: 10px 12px;
            font-size: 15px;
            font-weight: bold;
            text-align: center;
            text-decoration: none;
            cursor: pointer;
        }

        .danger button {
            background: #b3261e;
        }

        .toggle button {
            background: #5b2da0;
        }

        .display {
            background: #eef3f7;
            border-radius: 8px;
            padding: 10px;
            min-height: 42px;
            line-height: 1.45;
            margin-top: 10px;
            font-size: 14px;
            overflow-wrap: anywhere;
        }

        .ok {
            border-left: 5px solid #16833a;
        }

        .fail {
            border-left: 5px solid #b3261e;
        }

        .small {
            font-size: 12px;
            color: #666;
            line-height: 1.4;
        }

        .row {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
        }

        @media(max-width:700px) {
            .row {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <header>
        <h1>InverterModbusLib - Web Panel</h1>
        <nav><a href='/'>Painel</a><a href='/config'>Configuração</a></nav>
    </header>
    <div class='wrap'>
        <div id="inverterConfig" class='status'></div>
        </div>
        <div class='grid'>
            <div class='card'>
                <h2>Leituras - Identificação</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getSerialNumber'><button
                        type='submit'>getSerialNumber()</button></form>
                <div id="getSerialNumber" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getRatedPower'><button
                        type='submit'>getRatedPower()</button></form>
                <div id="getRatedPower" class='display small'>Aguardando leitura.</div>
                <h2>Escritas - Controle</h2>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn' value='boot'><button
                        type='submit'>boot()</button></form>
                <div id="boot" class="display small">Aguardando comando.</div>
                <div class='danger'>
                    <form onsubmit="callSet(event, this)"><input type='hidden' name='fn' value='shutdown'><button
                        type='submit'>shutdown()</button></form>
                <div id="shutdown" class="display small">Aguardando comando.</div>
                </div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setBoot'><label>Estado</label><select name='value'>
                        <option value='1'>true / habilitar</option>
                        <option value='0'>false / desabilitar</option>
                    </select><button type='submit'>setBoot()</button></form>
                <div id="setBoot" class="display small">Aguardando comando.</div>
            </div>
            <div class="card">
                <h2>Escritas - Limite de Potência</h2>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setPowerLimitEnabled'><label>Estado</label><select name='value'>
                        <option value='1'>true / habilitar</option>
                        <option value='0'>false / desabilitar</option>
                    </select><button type='submit'>setPowerLimitEnabled()</button></form>
                <div id="setPowerLimitEnabled" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn' value='setPowerLimit'><label>Valor
                        (W)</label><input type='number' step='1' name='value'><button
                        type='submit'>setPowerLimit()</button></form>
                <div id="setPowerLimit" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setPowerLimitPercent'><label>Valor (%)</label><input type='number' step='0.1'
                        name='value'><button type='submit'>setPowerLimitPercent()</button></form>
                <div id="setPowerLimitPercent" class="display small">Aguardando comando.</div>
                <h2>Leituras - Limite de Potência</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='isPowerLimitEnabled'><button
                        type='submit'>isPowerLimitEnabled()</button></form>
                <div id="isPowerLimitEnabled" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getPowerLimit'><button
                        type='submit'>getPowerLimit()</button></form>
                <div id="getPowerLimit" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getPowerLimitPercent'><button
                        type='submit'>getPowerLimitPercent()</button></form>
                <div id="getPowerLimitPercent" class='display small'>Aguardando leitura.</div>
            </div>
            <div class="card">
                <h2>Escritas - Limite de Exportação</h2>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setExportLimitEnabled'><label>Estado</label><select name='value'>
                        <option value='1'>true / habilitar</option>
                        <option value='0'>false / desabilitar</option>
                    </select><button type='submit'>setExportLimitEnabled()</button></form>
                <div id="setExportLimitEnabled" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn' value='setExportLimit'><label>Valor
                        (W)</label><input type='number' step='1' name='value'><button
                        type='submit'>setExportLimit()</button></form>
                <div id="setExportLimit" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setExportLimitPercent'><label>Valor (%)</label><input type='number' step='0.1'
                        name='value'><button type='submit'>setExportLimitPercent()</button></form>
                <div id="setExportLimitPercent" class="display small">Aguardando comando.</div>
                <h2>Leituras - Limite de Exportação</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='isExportLimitEnabled'><button
                        type='submit'>isExportLimitEnabled()</button></form>
                <div id="isExportLimitEnabled" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getExportLimit'><button
                        type='submit'>getExportLimit()</button></form>
                <div id="getExportLimit" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getExportLimitPercent'><button
                        type='submit'>getExportLimitPercent()</button></form>
                <div id="getExportLimitPercent" class='display small'>Aguardando leitura.</div>
            </div>
            <div class="card">
                <h2>Escritas - Fator de Potência</h2>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setPowerFactorEnabled'><label>Estado</label><select name='value'>
                        <option value='1'>true / habilitar</option>
                        <option value='0'>false / desabilitar</option>
                    </select><button type='submit'>setPowerFactorEnabled()</button></form>
                <div id="setPowerFactorEnabled" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setPowerFactorSetpoint'><label>Valor (%)</label><input type='number' step='0.01'
                        name='value'><button type='submit'>setPowerFactorSetpoint()</button></form>
                <div id="setPowerFactorSetpoint" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setPowerFactorExcitationMode'><label>Estado</label><select name='value'>
                        <option value='1'>Capacitivo</option>
                        <option value='0'>Indutivo</option>
                    </select><button type='submit'>setPowerFactorExcitationMode()</button></form>
                    <div id="setPowerFactorExcitationMode" class="display small">Aguardando comando.</div>
                <h2>Leituras - Fator de Potência</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='isPowerFactorEnabled'><button
                        type='submit'>isPowerFactorEnabled()</button></form>
                <div id="isPowerFactorEnabled" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getPowerFactorSetpoint'><button
                        type='submit'>getPowerFactorSetpoint()</button></form>
                <div id="getPowerFactorSetpoint" class='display small'>Aguardando leitura.</div>
            </div>
            <div class='card'>
                
                <h2>Escrita - Data e Hora</h2>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setYear'><input type='number' step='1'
                        name='value'><button type='submit'>setYear()</button></form>
                <div id="setYear" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setMonth'><input type='number' step='1'
                        name='value'><button type='submit'>setMonth()</button></form>
                <div id="setMonth" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setDay'><input type='number' step='1'
                        name='value'><button type='submit'>setDay()</button></form>
                <div id="setDay" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setHour'><input type='number' step='1'
                        name='value'><button type='submit'>setHour()</button></form>
                <div id="setHour" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setMinute'><input type='number' step='1'
                        name='value'><button type='submit'>setMinute()</button></form>
                <div id="setMinute" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setSecond'><input type='number' step='1'
                        name='value'><button type='submit'>setSecond()</button></form>
                <div id="setSecond" class="display small">Aguardando comando.</div>
                <form onsubmit="callSet(event, this)"><input type='hidden' name='fn'
                        value='setEpochTime'><input type='number' step='1'
                        name='value'><button type='submit'>setEpochTime()
                        </button></form>
                <div id="setEpochTime" class="display small">Aguardando comando.</div>
                <h2>Leituras - Data/Hora</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getYear'><button
                        type='submit'>getYear()</button></form>
                <div id="getYear" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getMonth'><button
                        type='submit'>getMonth()</button></form>
                <div id="getMonth" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getDay'><button
                        type='submit'>getDay()</button></form>
                <div id="getDay" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getHour'><button
                        type='submit'>getHour()</button></form>
                <div id="getHour" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getMinute'><button
                        type='submit'>getMinute()</button></form>
                <div id="getMinute" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getSecond'><button
                        type='submit'>getSecond()</button></form>
                <div id="getSecond" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getEpochTime'><button
                        type='submit'>getEpochTime()</button></form>
                <div id="getEpochTime" class='display small'>Aguardando leitura.</div>
            </div>
            <div class='card'>
                <h2>Leituras - Medições AC</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getActivePower'><button
                        type='submit'>getActivePower()</button></form>
                <div id="getActivePower" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getReactivePower'><button
                        type='submit'>getReactivePower()</button></form>
                <div id="getReactivePower" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getApparentPower'><button
                        type='submit'>getApparentPower()</button></form>
                <div id="getApparentPower" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getPowerFactor'><button
                        type='submit'>getPowerFactor()</button></form>
                <div id="getPowerFactor" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getGridVoltage'><button
                        type='submit'>getGridVoltage()</button></form>
                <div id="getGridVoltage" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getGridPhaseVoltage'><button
                        type='submit'>getGridPhaseVoltage()</button></form>
                <div id="getGridPhaseVoltage" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getGridLineVoltage'><button
                        type='submit'>getGridLineVoltage()</button></form>
                <div id="getGridLineVoltage" class='display small'>Aguardando leitura.</div> 
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getGridCurrent'><button
                        type='submit'>getGridCurrent()</button></form>
                <div id="getGridCurrent" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getGridFrequency'><button
                        type='submit'>getGridFrequency()</button></form>
                <div id="getGridFrequency" class='display small'>Aguardando leitura.</div>
                <h2>Leituras - Energia</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getTotalEnergy'><button
                        type='submit'>getTotalEnergy()</button></form>
                <div id="getTotalEnergy" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getDailyEnergy'><button
                        type='submit'>getDailyEnergy()</button></form>
                <div id="getDailyEnergy" class='display small'>Aguardando leitura.</div>
                <h2>Leituras - Strings FV</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getPVStringCount'><button
                        type='submit'>getPVStringCount()</button></form>
                <div id="getPVStringCount" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getStringVoltage'><button
                        type='submit'>getStringVoltage()</button></form>
                <div id="getStringVoltage" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getStringCurrent'><button
                        type='submit'>getStringCurrent()</button></form>
                <div id="getStringCurrent" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getStringPower'><button
                        type='submit'>getStringPower()</button></form>
                <div id="getStringPower" class='display small'>Aguardando leitura.</div>

            </div>
            <div class='card'>
                <h2>Leituras - Diagnóstico e Saúde</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getTemperature'><button
                        type='submit'>getTemperature()</button></form>
                <div id="getTemperature" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getInsulationResistance'><button
                        type='submit'>getInsulationResistance()</button></form>
                <div id="getInsulationResistance" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getInverterStatus'><button
                        type='submit'>getInverterStatus()</button></form>
                <div id="getInverterStatus" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getAlarm'><button
                        type='submit'>getAlarm()</button></form>
                <div id="getAlarm" class='display small'>Aguardando leitura.</div>
            </div>
            <div class="card">
                <h2>Leituras - Baterias</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getBatteryVoltage'><button
                        type='submit'>getBatteryVoltage()</button></form>
                <div id="getBatteryVoltage" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getBatteryCurrent'><button
                        type='submit'>getBatteryCurrent()</button></form>
                <div id="getBatteryCurrent" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getBatteryPower'><button
                        type='submit'>getBatteryPower()</button></form>
                <div id="getBatteryPower" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getBatterySoC'><button
                        type='submit'>getBatterySoC()</button></form>
                <div id="getBatterySoC" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getBatterySoH'><button
                        type='submit'>getBatterySoH()</button></form>
                <div id="getBatterySoH" class='display small'>Aguardando leitura.</div>
                <h2>Leituras - EPS</h2>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getEPSVoltage'><button
                        type='submit'>getEPSVoltage()</button></form>
                <div id="getEPSVoltage" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getEPSCurrent'><button
                        type='submit'>getEPSCurrent()</button></form>
                <div id="getEPSCurrent" class='display small'>Aguardando leitura.</div>
                <form onsubmit="callGet(event, this)"><input type='hidden' name='fn' value='getEPSActivePower'><button
                        type='submit'>getEPSActivePower()</button></form>
                <div id="getEPSActivePower" class='display small'>Aguardando leitura.</div>
            </div>
        </div>
    </div>
    <script>
        async function callGet(event, form) {
                event.preventDefault();

                const formData = new FormData(form);
                const params = new URLSearchParams(formData);
                const fn = formData.get("fn");

                const response = await fetch("/api/get?" + params.toString());
                const text = await response.text();

                const element = document.getElementById(fn);
                if (element) element.innerHTML = text;
                }

                async function callSet(event, form) {
                event.preventDefault();

                const formData = new FormData(form);
                const params = new URLSearchParams(formData);
                const fn = formData.get("fn");

                const response = await fetch("/api/set?" + params.toString());
                const text = await response.text();

                const element = document.getElementById(fn);
                if (element) element.innerHTML = text;
        }

        async function loadConfig() {
                const response = await fetch("/api/config");
                const text = await response.text();
                document.getElementById("inverterConfig").innerHTML = text;
        }

        window.onload = loadConfig;
    </script>
</body>

</html>)rawliteral";

const char configPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <title>Configuração Modbus</title>
    <style>
        body {
            font-family: Arial, Helvetica, sans-serif;
            background: #f4f6f8;
            margin: 0;
            color: #222;
        }

        header {
            background: #0b3d5c;
            color: white;
            padding: 16px 20px;
        }

        header h1 {
            font-size: 22px;
            margin: 0 0 10px 0;
        }

        nav a {
            display: inline-block;
            color: white;
            text-decoration: none;
            background: #1565c0;
            padding: 8px 12px;
            border-radius: 8px;
            margin-right: 8px;
        }

        .wrap {
            max-width: 1180px;
            margin: 0 auto;
            padding: 18px;
        }

        .status {
            background: #fff;
            border-left: 5px solid #1565c0;
            padding: 12px;
            border-radius: 10px;
            margin-bottom: 16px;
            box-shadow: 0 1px 5px rgba(0, 0, 0, .08);
        }

        .grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 14px;
        }

        .card {
            background: white;
            border-radius: 12px;
            padding: 14px;
            box-shadow: 0 2px 8px rgba(0, 0, 0, .12);
        }

        .card h2 {
            font-size: 17px;
            margin: 0 0 10px 0;
            color: #0b3d5c;
        }

        form {
            margin: 0 0 10px 0;
        }

        label {
            display: block;
            font-size: 13px;
            font-weight: bold;
            margin-top: 8px;
            margin-bottom: 4px;
        }

        input,
        select {
            width: 100%;
            box-sizing: border-box;
            padding: 9px;
            border: 1px solid #bbb;
            border-radius: 8px;
            font-size: 15px;
            background: white;
        }

        button,
        .btn {
            display: inline-block;
            width: 100%;
            box-sizing: border-box;
            border: 0;
            border-radius: 8px;
            background: #1565c0;
            color: white;
            padding: 10px 12px;
            font-size: 15px;
            font-weight: bold;
            text-align: center;
            text-decoration: none;
            cursor: pointer;
        }

        .danger button {
            background: #b3261e;
        }

        .toggle button {
            background: #5b2da0;
        }

        .display {
            background: #eef3f7;
            border-radius: 8px;
            padding: 10px;
            min-height: 42px;
            line-height: 1.45;
            margin-top: 10px;
            font-size: 14px;
            overflow-wrap: anywhere;
        }

        .ok {
            border-left: 5px solid #16833a;
        }

        .fail {
            border-left: 5px solid #b3261e;
        }

        .small {
            font-size: 12px;
            color: #666;
            line-height: 1.4;
        }

        .row {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
        }

        @media(max-width:700px) {
            .row {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>

<body>
    <header>
        <h1>Configuração Modbus</h1>
        <nav><a href='/'>Painel</a><a href='/config'>Configuração</a></nav>
    </header>
    <div class='wrap'>
        <div class='card'>
            <form action='/config/apply' method='GET'><label>Modelo do inversor</label><select name='model'>
                    <option value='0' selected>0 - SIW200_M030</option>
                    <option value='1'>1 - SIW200_M050</option>
                    <option value='2'>2 - SIW200_M085</option>
                    <option value='3'>3 - SIW200_M100</option>
                    <option value='4'>4 - SIW200H_M050_W00</option>
                    <option value='5'>5 - SIW200H_M075_W10</option>
                    <option value='6'>6 - SIW200H_M105_W10</option>
                    <option value='7'>7 - SIW200G_M030_W0</option>
                    <option value='8'>8 - SIW200G_M030_W1</option>
                    <option value='9'>9 - SIW200G_M050_W0</option>
                    <option value='10'>10 - SIW200G_M050_W1</option>
                    <option value='11'>11 - SIW200G_M060_W0</option>
                    <option value='12'>12 - SIW200G_M060_W1</option>
                    <option value='13'>13 - SIW200G_M070_W1</option>
                    <option value='14'>14 - SIW200G_M075_W1</option>
                    <option value='15'>15 - SIW200G_M080_W1</option>
                    <option value='16'>16 - SIW200G_M090_W1</option>
                    <option value='17'>17 - SIW200G_M105_W1</option>
                    <option value='18'>18 - SIW300H_M030_W00</option>
                    <option value='19'>19 - SIW300H_M050_W00</option>
                    <option value='20'>20 - SIW300H_M060_W00</option>
                    <option value='21'>21 - SIW300H_M075_W00</option>
                    <option value='22'>22 - SIW300H_M080_W00</option>
                    <option value='23'>23 - SIW300H_M100_W00</option>
                    <option value='24'>24 - SIW400_ST075</option>
                    <option value='25'>25 - SIW400G_T012_W0</option>
                    <option value='26'>26 - SIW400G_T012_W1</option>
                    <option value='27'>27 - SIW400G_T015_W0</option>
                    <option value='28'>28 - SIW400G_T015_W1</option>
                    <option value='29'>29 - SIW400G_T020_W0</option>
                    <option value='30'>30 - SIW400G_T020_W1</option>
                    <option value='31'>31 - SIW400G_T025_W0</option>
                    <option value='32'>32 - SIW400G_T025_W1</option>
                    <option value='33'>33 - SIW400G_T075_W0</option>
                    <option value='34'>34 - SIW400G_T100_W0</option>
                    <option value='35'>35 - SIW400G_T050_W00</option>
                    <option value='36'>36 - SIW400G_T060_W00</option>
                    <option value='37'>37 - SIW400G_T075_W01</option>
                    <option value='38'>38 - SIW400G_K015_W00</option>
                    <option value='39'>39 - SIW400G_K020_W00</option>
                    <option value='40'>40 - SIW400G_K025_W00</option>
                    <option value='41'>41 - SIW400G_K030_W00</option>
                    <option value='42'>42 - SIW400G_K037_W00</option>
                    <option value='43'>43 - SIW400H_T015_W10</option>
                    <option value='44'>44 - SIW400H_T030_W10</option>
                    <option value='45'>45 - SIW420G_K025_W00</option>
                    <option value='46'>46 - SIW420G_K075_W00</option>
                    <option value='47'>47 - SIW500G_T075_W0</option>
                    <option value='48'>48 - SIW500G_T100_W0</option>
                    <option value='49'>49 - SIW500G_K050_W00</option>
                    <option value='50'>50 - SIW500G_K075_W00</option>
                    <option value='51'>51 - SIW500G_H250_W0</option>
                    <option value='52'>52 - SIW500H_ST012_M2</option>
                    <option value='53'>53 - SIW500H_ST015_M2</option>
                    <option value='54'>54 - SIW500H_ST020_M2</option>
                    <option value='55'>55 - SIW500H_ST030_M3</option>
                    <option value='56'>56 - SIW500H_ST036_M3</option>
                    <option value='57'>57 - SIW500H_ST040_M3</option>
                    <option value='58'>58 - SIW500H_SK020</option>
                    <option value='59'>59 - SIW500H_ST030</option>
                    <option value='60'>60 - SIW500H_ST036</option>
                    <option value='61'>61 - SIW500H_ST040</option>
                    <option value='62'>62 - SIW500H_ST060</option>
                    <option value='63'>63 - SIW500H_ST100</option>
                    <option value='64'>64 - SIW500H_ST060_HV</option>
                    <option value='65'>65 - SIW500H_ST100_HV</option>
                    <option value='66'>66 - SIW500H_ST200_H0</option>
                    <option value='67'>67 - SIW500H_ST200_H3</option>
                    <option value='68'>68 - SIW500H_T012_W00</option>
                    <option value='69'>69 - SIW500H_T015_W00</option>
                    <option value='70'>70 - SIW500H_T017_W00</option>
                    <option value='71'>71 - SIW500H_T020_W00</option>
                    <option value='72'>72 - SIW500H_T025_W00</option>
                    <option value='73'>73 - SIW600_T020_44</option>
                    <option value='74'>74 - SIW610_T018_W0</option>
                    <option value='75'>75 - SIW610_T075_W0</option>
                    <option value='76'>76 - S700_G2</option>
                    <option value='77'>77 - S1000_G2</option>
                    <option value='78'>78 - S1500_G2</option>
                    <option value='79'>79 - S2000_G2</option>
                    <option value='80'>80 - S2500_G2</option>
                    <option value='81'>81 - S3000_G2</option>
                    <option value='82'>82 - S3300_G2</option>
                    <option value='83'>83 - F3000_G2</option>
                    <option value='84'>84 - F3600_G2</option>
                    <option value='85'>85 - F4600_G2</option>
                    <option value='86'>86 - F5000_G2</option>
                    <option value='87'>87 - F5300_G2</option>
                    <option value='88'>88 - F6000_G2</option>
                    <option value='89'>89 - G7</option>
                    <option value='90'>90 - G7_5</option>
                    <option value='91'>91 - G8_</option>
                    <option value='92'>92 - G9</option>
                    <option value='93'>93 - G10</option>
                    <option value='94'>94 - G10_5</option>
                    <option value='95'>95 - T3_G3</option>
                    <option value='96'>96 - T4_G3</option>
                    <option value='97'>97 - T5_G3</option>
                    <option value='98'>98 - T6_G3</option>
                    <option value='99'>99 - T8_G3</option>
                    <option value='100'>100 - T10_G3</option>
                    <option value='101'>101 - T12_G3</option>
                    <option value='102'>102 - T15_G3</option>
                    <option value='103'>103 - T17_G3</option>
                    <option value='104'>104 - T20_G3</option>
                    <option value='105'>105 - T23_G3</option>
                    <option value='106'>106 - T25_G3</option>
                    <option value='107'>107 - V30</option>
                    <option value='108'>108 - V33</option>
                    <option value='109'>109 - V36</option>
                    <option value='110'>110 - V40</option>
                    <option value='111'>111 - V50</option>
                    <option value='112'>112 - V60</option>
                    <option value='113'>113 - V75</option>
                    <option value='114'>114 - VL15</option>
                    <option value='115'>115 - VL20</option>
                    <option value='116'>116 - VL25</option>
                    <option value='117'>117 - VL30</option>
                    <option value='118'>118 - VL37_5</option>
                    <option value='119'>119 - R75</option>
                    <option value='120'>120 - R100</option>
                    <option value='121'>121 - R110</option>
                    <option value='122'>122 - GW3000_XS</option>
                    <option value='123'>123 - GW3300_XS_30</option>
                    <option value='124'>124 - GW5000_DNS</option>
                    <option value='125'>125 - GW5K_DNS_G40</option>
                    <option value='126'>126 - GW8500_MS</option>
                    <option value='127'>127 - GW10K_MS</option>
                    <option value='128'>128 - SUN2000_2KTL</option>
                    <option value='129'>129 - SUN2000_3KTL</option>
                    <option value='130'>130 - SUN2000_4KTL</option>
                    <option value='131'>131 - SUN2000_5KTL</option>
                    <option value='132'>132 - SUN2000_6KTL</option>
                    <option value='133'>133 - SUN2000_12KTL_M0</option>
                    <option value='134'>134 - SUN2000_12KTL_M2</option>
                    <option value='135'>135 - SUN2000_15KTL_M0</option>
                    <option value='136'>136 - SUN2000_15KTL_M2</option>
                    <option value='137'>137 - SUN2000_20KTL_M0</option>
                    <option value='138'>138 - SUN2000_20KTL_M2</option>
                    <option value='139'>139 - SUN2000_30KTL</option>
                    <option value='140'>140 - SUN2000_36KTL_A</option>
                    <option value='141'>141 - SUN2000_30KTL_M3</option>
                    <option value='142'>142 - SUN2000_36KTL_M3</option>
                    <option value='143'>143 - SUN2000_40KTL_M3</option>
                    <option value='144'>144 - SUN2000_60KTL_M0</option>
                    <option value='145'>145 - SUN2000_75KTL_M1</option>
                    <option value='146'>146 - SUN2000_100KTL_M2</option>
                    <option value='147'>147 - SUN2000_100KTL_H1</option>
                    <option value='148'>148 - SUN2000_215KTL_H0</option>
                    <option value='149'>149 - SUN2000_215KTL_H3</option>
                    <option value='150'>150 - SUN2000_250KTL_H1</option>
                </select>
                <div class='row'>
                    <div><label>Slave ID</label><input type='number' name='id' min='1' max='247' value='1'></div>
                    <div><label>Baud rate</label><select name='baud'>
                            <option value='1200'>1200</option>
                            <option value='2400'>2400</option>
                            <option value='4800'>4800</option>
                            <option value='9600' selected>9600</option>
                            <option value='19200'>19200</option>
                            <option value='38400'>38400</option>
                            <option value='57600'>57600</option>
                            <option value='115200'>115200</option>
                        </select></div>
                    <div><label>Paridade</label><select name='parity'>
                            <option value='N'>None</option>
                            <option value='E'>Even</option>
                            <option value='O'>Odd</option>
                        </select></div>
                    <div><label>Stop bits</label><select name='stopBits'>
                            <option value='1'>1</option>
                            <option value='2'>2</option>
                        </select></div>
                    <div><label>Pino DE/RE</label><input type='number' name='dere' min='-1' max='16' value='-1'></div>
                </div>
                <p class='small'>Data bits fixo em 8. Use DE/RE = -1 apenas se não houver controle RS485 por pino.</p>
                <button type='submit'>Atualizar Serial/Modbus</button>
            </form>
        </div>
    </div>
</body>

</html>
)rawliteral";