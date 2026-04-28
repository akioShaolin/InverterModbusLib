#include "InverterDescriptor.h"
#include "InverterModels.h"

// Revisar os alarmes e status depois. Linha SIW200 ok. SIW300H ok
// Weg
constexpr InverterDescriptor desc_SIW200_M030 PROGMEM =            {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x00, 0x00}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200_M050 PROGMEM =            {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x00, 0x00}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200_M085 PROGMEM =            {SINGLE_PHASE, STRING, ON_GRID, 8500U,   {0x00, 0x00}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200_M100 PROGMEM =            {SINGLE_PHASE, STRING, ON_GRID, 10000U,  {0x00, 0x00}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200H_M050_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {4, 1, {0, 0, 0, 0}},             SINGLE_PHASE,   AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200H_M075_W10 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  7500U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {4, 1, {0, 0, 0, 0}},             SINGLE_PHASE,   AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200H_M105_W10 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  10500U,  {0x01, 0x01}, {4, 4,   {0, 1, 2, 3}},                                                                         {4, 1, {0, 0, 0, 0}},             SINGLE_PHASE,   AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M030_W0 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M030_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M050_W0 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M050_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M060_W0 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 6000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M060_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 6000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M070_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 7000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M075_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 7500U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M080_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 8000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M090_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 9000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW200G_M105_W1 PROGMEM =        {SINGLE_PHASE, STRING, ON_GRID, 10500U,  {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW300H_M030_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  3000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {2, 1, {0, 0}},                   SINGLE_PHASE,   AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW300H_M050_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {2, 1, {0, 0}},                   SINGLE_PHASE,   AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW300H_M060_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  6000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {2, 1, {0, 0}},                   SINGLE_PHASE,   AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW300H_M075_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  7500U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {2, 1, {0, 0}},                   PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW300H_M080_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  8000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {2, 1, {0, 0}},                   PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW300H_M100_W00 PROGMEM =       {SINGLE_PHASE, STRING, HYBRID,  10000U,  {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {2, 1, {0, 0}},                   PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW400_ST075 PROGMEM =           {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0x00, 0x00}, {16, 4,  {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3}},                                     {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T012_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 12000U,  {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T012_W1 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 12000U,  {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T015_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T015_W1 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T020_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T020_W1 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T025_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 25000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T025_W1 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 25000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T075_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0xCE, 0xCF}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T100_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 100000U, {0xCE, 0xCF}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T050_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 50000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T060_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 60000U,  {0xCE, 0xCF}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_T075_W01 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0xCE, 0xCF}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K015_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K020_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K025_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 25000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K030_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 30000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400G_K037_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 37500U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400H_T015_W10 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  15000U,  {0xCE, 0xCF}, {6, 3,   {0, 0, 1, 1, 2, 2}},                                                                   {8, 2, {0, 0, 0, 0, 1, 1, 1, 1}}, THREE_PHASE,    AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW400H_T030_W10 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  30000U,  {0xCE, 0xCF}, {6, 3,   {0, 0, 1, 1, 2, 2}},                                                                   {8, 2, {0, 0, 0, 0, 1, 1, 1, 1}}, THREE_PHASE,    AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW420G_K025_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 25000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::UNKNOWN,    StatusFormat::UNKNOWN    };
constexpr InverterDescriptor desc_SIW420G_K075_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0x01, 0x01}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::UNKNOWN,    StatusFormat::UNKNOWN    };
constexpr InverterDescriptor desc_SIW500G_T075_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0x01, 0x01}, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}},                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_T100_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 100000U, {0x01, 0x01}, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}},                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_K050_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 50000U,  {0x01, 0x01}, {21, 7,  {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}},                      {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_K075_W00 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0x01, 0x01}, {21, 7,  {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}},                      {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500G_H250_W0 PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 250000U, {0x01, 0x01}, {28, 6,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5}}, {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST012_M2 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 12000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST015_M2 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST020_M2 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST030_M3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 30000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST036_M3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 36000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST040_M3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 40000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_SK020 PROGMEM =          {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST030 PROGMEM =          {THREE_PHASE,  STRING, ON_GRID, 30000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST036 PROGMEM =          {THREE_PHASE,  STRING, ON_GRID, 36000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST040 PROGMEM =          {THREE_PHASE,  STRING, ON_GRID, 40000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST060 PROGMEM =          {THREE_PHASE,  STRING, ON_GRID, 60000U,  {0x01, 0x01}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST100 PROGMEM =          {THREE_PHASE,  STRING, ON_GRID, 100000U, {0x01, 0x01}, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}},                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST200_H3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 200000U, {0x01, 0x01}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST200_H0 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 200000U, {0x01, 0x01}, {14, 3,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2}},                                           {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST060_HV PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 600000U, {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_ST100_HV PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 100000U, {0x01, 0x01}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T012_W00 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  12000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T015_W00 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T017_W00 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  17000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T020_W00 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW500H_T025_W00 PROGMEM =       {THREE_PHASE,  STRING, HYBRID,  25000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SIW600_T020_44 PROGMEM =         {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x00}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW610_T018_W0 PROGMEM =         {THREE_PHASE,  STRING, ON_GRID, 18000U,  {0x01, 0x00}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SIW610_T075_W0 PROGMEM =         {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0x01, 0x00}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
// Foxess
constexpr InverterDescriptor desc_S700_G2 PROGMEM =                {SINGLE_PHASE, STRING, ON_GRID, 700U,    {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S1000_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 1000U,   {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S1500_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 1500U,   {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S2000_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 2000U,   {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S2500_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 2500U,   {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };;
constexpr InverterDescriptor desc_S3000_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_S3300_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 3300U,   {0x01, 0x01}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F3000_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F3600_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 3600U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F4600_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 4600U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F5000_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F5300_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 5300U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_F6000_G2 PROGMEM =               {SINGLE_PHASE, STRING, ON_GRID, 6000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G7 PROGMEM =                     {SINGLE_PHASE, STRING, ON_GRID, 7000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G7_5 PROGMEM =                   {SINGLE_PHASE, STRING, ON_GRID, 7500U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G8 PROGMEM =                     {SINGLE_PHASE, STRING, ON_GRID, 8000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G9 PROGMEM =                     {SINGLE_PHASE, STRING, ON_GRID, 9000U,   {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G10 PROGMEM =                    {SINGLE_PHASE, STRING, ON_GRID, 10000U,  {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_G10_5 PROGMEM =                  {SINGLE_PHASE, STRING, ON_GRID, 10500U,  {0x01, 0x01}, {3, 3,   {0, 1, 2}},                                                                            {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T3_G3 PROGMEM =                  {THREE_PHASE,  STRING, ON_GRID, 3000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T4_G3 PROGMEM =                  {THREE_PHASE,  STRING, ON_GRID, 4000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T5_G3 PROGMEM =                  {THREE_PHASE,  STRING, ON_GRID, 5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T6_G3 PROGMEM =                  {THREE_PHASE,  STRING, ON_GRID, 6000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T8_G3 PROGMEM =                  {THREE_PHASE,  STRING, ON_GRID, 8000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T10_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 10000U,  {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T12_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 12000U,  {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T15_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T17_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 17000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T20_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T23_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 23000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_T25_G3 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 25000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V30 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 30000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V33 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 33000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V36 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 36000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V40 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 40000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V50 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 50000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V60 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 60000U,  {0xCE, 0xCF}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_V75 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0xCE, 0xCF}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL15 PROGMEM =                   {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL20 PROGMEM =                   {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL25 PROGMEM =                   {THREE_PHASE,  STRING, ON_GRID, 25000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL30 PROGMEM =                   {THREE_PHASE,  STRING, ON_GRID, 30000U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_VL37_5 PROGMEM =                 {THREE_PHASE,  STRING, ON_GRID, 37500U,  {0xCE, 0xCF}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_R75 PROGMEM =                    {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0xCE, 0xCF}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_R100 PROGMEM =                   {THREE_PHASE,  STRING, ON_GRID, 100000U, {0xCE, 0xCF}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_R110 PROGMEM =                   {THREE_PHASE,  STRING, ON_GRID, 110000U, {0xCE, 0xCF}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
//Goodwe
constexpr InverterDescriptor desc_GW3000_XS PROGMEM =              {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x00, 0x00}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_GW3300_XS_30 PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 3300U,   {0x00, 0x00}, {1, 1,   {0}},                                                                                  {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_GW5000_DNS PROGMEM =             {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x00, 0x00}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_GW5K_DNS_G40 PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x00, 0x00}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD32, StatusFormat::ENUM_CODE  };
//GW8500-MS, GW10K-MS
// Huawei
constexpr InverterDescriptor desc_SUN2000_2KTL PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 2000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_3KTL PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 3000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_4KTL PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 4000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_5KTL PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 5000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_6KTL PROGMEM =           {SINGLE_PHASE, STRING, ON_GRID, 6000U,   {0x01, 0x01}, {2, 2,   {0, 1}},                                                                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::ENUM_CODE  };
constexpr InverterDescriptor desc_SUN2000_12KTL_M0 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 12000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_12KTL_M2 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 12000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_15KTL_M0 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_15KTL_M2 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 15000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_20KTL_M0 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_20KTL_M2 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 20000U,  {0x01, 0x01}, {4, 2,   {0, 0, 1, 1}},                                                                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_33KTL_A PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 33000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_36KTL_A PROGMEM =        {THREE_PHASE,  STRING, ON_GRID, 36000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_30KTL_M3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 30000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_36KTL_M3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 36000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_40KTL_M3 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 40000U,  {0x01, 0x01}, {8, 4,   {0, 0, 1, 1, 2, 2, 3, 3}},                                                             {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_60KTL_M0 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 60000U,  {0x01, 0x01}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_75KTL_M1 PROGMEM =       {THREE_PHASE,  STRING, ON_GRID, 75000U,  {0x01, 0x01}, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}},                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_100KTL_M2 PROGMEM =      {THREE_PHASE,  STRING, ON_GRID, 100000U, {0x01, 0x01}, {20, 10, {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9}},                         {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_100KTL_H1 PROGMEM =      {THREE_PHASE,  STRING, ON_GRID, 100000U, {0x01, 0x01}, {12, 6,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5}},                                                 {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_215KTL_H0 PROGMEM =      {THREE_PHASE,  STRING, ON_GRID, 215000U, {0x01, 0x01}, {18, 9,  {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8}},                               {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_215KTL_H3 PROGMEM =      {THREE_PHASE,  STRING, ON_GRID, 215000U, {0x01, 0x01}, {14, 3,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2}},                                           {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };
constexpr InverterDescriptor desc_SUN2000_250KTL_H1 PROGMEM =      {THREE_PHASE,  STRING, ON_GRID, 250000U, {0x01, 0x01}, {28, 6,  {0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5}}, {0, 0, {0}},                      PHASE_NONE,     AlarmFormat::BITFIELD16, StatusFormat::BITFIELD16 };

InverterDescriptor getDescriptor(InverterModel model) {
    InverterDescriptor desc;

    switch (model) {
        case SIW200_M030:
            memcpy_P(&desc, &desc_SIW200_M030, sizeof(InverterDescriptor));
            return desc;
        case SIW200_M050:
            memcpy_P(&desc, &desc_SIW200_M050, sizeof(InverterDescriptor));
            return desc;
        case SIW200_M085:
            memcpy_P(&desc, &desc_SIW200_M085, sizeof(InverterDescriptor));
            return desc;
        case SIW200_M100:
            memcpy_P(&desc, &desc_SIW200_M100, sizeof(InverterDescriptor));
            return desc;
        case SIW200H_M050_W00:
            memcpy_P(&desc, &desc_SIW200H_M050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW200H_M075_W10:
            memcpy_P(&desc, &desc_SIW200H_M075_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW200H_M105_W10:
            memcpy_P(&desc, &desc_SIW200H_M105_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M030_W0:
            memcpy_P(&desc, &desc_SIW200G_M030_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M030_W1:
            memcpy_P(&desc, &desc_SIW200G_M030_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M050_W0:
            memcpy_P(&desc, &desc_SIW200G_M050_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M050_W1:
            memcpy_P(&desc, &desc_SIW200G_M050_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M060_W0:
            memcpy_P(&desc, &desc_SIW200G_M060_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M060_W1:
            memcpy_P(&desc, &desc_SIW200G_M060_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M070_W1:
            memcpy_P(&desc, &desc_SIW200G_M070_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M075_W1:
            memcpy_P(&desc, &desc_SIW200G_M075_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M080_W1:
            memcpy_P(&desc, &desc_SIW200G_M080_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M090_W1:
            memcpy_P(&desc, &desc_SIW200G_M090_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW200G_M105_W1:
            memcpy_P(&desc, &desc_SIW200G_M105_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M030_W00:
            memcpy_P(&desc, &desc_SIW300H_M030_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M050_W00:
            memcpy_P(&desc, &desc_SIW300H_M050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M060_W00:
            memcpy_P(&desc, &desc_SIW300H_M060_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M075_W00:
            memcpy_P(&desc, &desc_SIW300H_M075_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M080_W00:
            memcpy_P(&desc, &desc_SIW300H_M080_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW300H_M100_W00:
            memcpy_P(&desc, &desc_SIW300H_M100_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400_ST075:
            memcpy_P(&desc, &desc_SIW400_ST075, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T012_W0:
            memcpy_P(&desc, &desc_SIW400G_T012_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T012_W1:
            memcpy_P(&desc, &desc_SIW400G_T012_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T015_W0:
            memcpy_P(&desc, &desc_SIW400G_T015_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T015_W1:
            memcpy_P(&desc, &desc_SIW400G_T015_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T020_W0:
            memcpy_P(&desc, &desc_SIW400G_T020_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T020_W1:
            memcpy_P(&desc, &desc_SIW400G_T020_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T025_W0:
            memcpy_P(&desc, &desc_SIW400G_T025_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T025_W1:
            memcpy_P(&desc, &desc_SIW400G_T025_W1, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T075_W0:
            memcpy_P(&desc, &desc_SIW400G_T075_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T100_W0:
            memcpy_P(&desc, &desc_SIW400G_T100_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T050_W00:
            memcpy_P(&desc, &desc_SIW400G_T050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T060_W00:
            memcpy_P(&desc, &desc_SIW400G_T060_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_T075_W01:
            memcpy_P(&desc, &desc_SIW400G_T075_W01, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K015_W00:
            memcpy_P(&desc, &desc_SIW400G_K015_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K020_W00:
            memcpy_P(&desc, &desc_SIW400G_K020_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K025_W00:
            memcpy_P(&desc, &desc_SIW400G_K025_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K030_W00:
            memcpy_P(&desc, &desc_SIW400G_K030_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400G_K037_W00:
            memcpy_P(&desc, &desc_SIW400G_K037_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW400H_T015_W10:
            memcpy_P(&desc, &desc_SIW400H_T015_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW400H_T030_W10:
            memcpy_P(&desc, &desc_SIW400H_T030_W10, sizeof(InverterDescriptor));
            return desc;
        case SIW420G_K025_W00:
            memcpy_P(&desc, &desc_SIW420G_K025_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW420G_K075_W00:
            memcpy_P(&desc, &desc_SIW420G_K075_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_T075_W0:
            memcpy_P(&desc, &desc_SIW500G_T075_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_T100_W0:
            memcpy_P(&desc, &desc_SIW500G_T100_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_K050_W00:
            memcpy_P(&desc, &desc_SIW500G_K050_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_K075_W00:
            memcpy_P(&desc, &desc_SIW500G_K075_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500G_H250_W0:
            memcpy_P(&desc, &desc_SIW500G_H250_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST012_M2:
            memcpy_P(&desc, &desc_SIW500H_ST012_M2, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST015_M2:
            memcpy_P(&desc, &desc_SIW500H_ST015_M2, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST020_M2:
            memcpy_P(&desc, &desc_SIW500H_ST020_M2, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST030_M3:
            memcpy_P(&desc, &desc_SIW500H_ST030_M3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST036_M3:
            memcpy_P(&desc, &desc_SIW500H_ST036_M3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST040_M3:
            memcpy_P(&desc, &desc_SIW500H_ST040_M3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_SK020:
            memcpy_P(&desc, &desc_SIW500H_SK020, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST030:
            memcpy_P(&desc, &desc_SIW500H_ST030, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST036:
            memcpy_P(&desc, &desc_SIW500H_ST036, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST040:
            memcpy_P(&desc, &desc_SIW500H_ST040, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST060:
            memcpy_P(&desc, &desc_SIW500H_ST060, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST100:
            memcpy_P(&desc, &desc_SIW500H_ST100, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST060_HV:
            memcpy_P(&desc, &desc_SIW500H_ST060_HV, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST100_HV:
            memcpy_P(&desc, &desc_SIW500H_ST100_HV, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST200_H0:
            memcpy_P(&desc, &desc_SIW500H_ST200_H0, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_ST200_H3:
            memcpy_P(&desc, &desc_SIW500H_ST200_H3, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T012_W00:
            memcpy_P(&desc, &desc_SIW500H_T012_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T015_W00:
            memcpy_P(&desc, &desc_SIW500H_T015_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T017_W00:
            memcpy_P(&desc, &desc_SIW500H_T017_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T020_W00:
            memcpy_P(&desc, &desc_SIW500H_T020_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW500H_T025_W00:
            memcpy_P(&desc, &desc_SIW500H_T025_W00, sizeof(InverterDescriptor));
            return desc;
        case SIW600_T020_44:
            memcpy_P(&desc, &desc_SIW600_T020_44, sizeof(InverterDescriptor));
            return desc;
        case SIW610_T018_W0:
            memcpy_P(&desc, &desc_SIW610_T018_W0, sizeof(InverterDescriptor));
            return desc;
        case SIW610_T075_W0:
            memcpy_P(&desc, &desc_SIW610_T075_W0, sizeof(InverterDescriptor));
            return desc;
        case S700_G2:
            memcpy_P(&desc, &desc_S700_G2, sizeof(InverterDescriptor));
            return desc;
        case S1000_G2:
            memcpy_P(&desc, &desc_S1000_G2, sizeof(InverterDescriptor));
            return desc;
        case S1500_G2:
            memcpy_P(&desc, &desc_S1500_G2, sizeof(InverterDescriptor));
            return desc;
        case S2000_G2:
            memcpy_P(&desc, &desc_S2000_G2, sizeof(InverterDescriptor));
            return desc;
        case S2500_G2:
            memcpy_P(&desc, &desc_S2500_G2, sizeof(InverterDescriptor));
            return desc;
        case S3000_G2:
            memcpy_P(&desc, &desc_S3000_G2, sizeof(InverterDescriptor));
            return desc;
        case S3300_G2:
            memcpy_P(&desc, &desc_S3300_G2, sizeof(InverterDescriptor));
            return desc;
        case F3000_G2:
            memcpy_P(&desc, &desc_F3000_G2, sizeof(InverterDescriptor));
            return desc;
        case F3600_G2:
            memcpy_P(&desc, &desc_F3600_G2, sizeof(InverterDescriptor));
            return desc;
        case F4600_G2:
            memcpy_P(&desc, &desc_F4600_G2, sizeof(InverterDescriptor));
            return desc;
        case F5000_G2:
            memcpy_P(&desc, &desc_F5000_G2, sizeof(InverterDescriptor));
            return desc;
        case F5300_G2:
            memcpy_P(&desc, &desc_F5300_G2, sizeof(InverterDescriptor));
            return desc;
        case F6000_G2:
            memcpy_P(&desc, &desc_F6000_G2, sizeof(InverterDescriptor));
            return desc;
        case G7:
            memcpy_P(&desc, &desc_G7, sizeof(InverterDescriptor));
            return desc;
        case G7_5:
            memcpy_P(&desc, &desc_G7_5, sizeof(InverterDescriptor));
            return desc;
        case G8_:
            memcpy_P(&desc, &desc_G8, sizeof(InverterDescriptor));
            return desc;
        case G9:
            memcpy_P(&desc, &desc_G9, sizeof(InverterDescriptor));
            return desc;
        case G10:
            memcpy_P(&desc, &desc_G10, sizeof(InverterDescriptor));
            return desc;
        case G10_5:
            memcpy_P(&desc, &desc_G10_5, sizeof(InverterDescriptor));
            return desc;
        case T3_G3:
            memcpy_P(&desc, &desc_T3_G3, sizeof(InverterDescriptor));
            return desc;
        case T4_G3:
            memcpy_P(&desc, &desc_T4_G3, sizeof(InverterDescriptor));
            return desc;
        case T5_G3:
            memcpy_P(&desc, &desc_T5_G3, sizeof(InverterDescriptor));
            return desc;
        case T6_G3:
            memcpy_P(&desc, &desc_T6_G3, sizeof(InverterDescriptor));
            return desc;
        case T8_G3:
            memcpy_P(&desc, &desc_T8_G3, sizeof(InverterDescriptor));
            return desc;
        case T10_G3:
            memcpy_P(&desc, &desc_T10_G3, sizeof(InverterDescriptor));
            return desc;
        case T12_G3:
            memcpy_P(&desc, &desc_T12_G3, sizeof(InverterDescriptor));
            return desc;
        case T15_G3:
            memcpy_P(&desc, &desc_T15_G3, sizeof(InverterDescriptor));
            return desc;
        case T17_G3:
            memcpy_P(&desc, &desc_T17_G3, sizeof(InverterDescriptor));
            return desc;
        case T20_G3:
            memcpy_P(&desc, &desc_T20_G3, sizeof(InverterDescriptor));
            return desc;
        case T23_G3:
            memcpy_P(&desc, &desc_T23_G3, sizeof(InverterDescriptor));
            return desc;
        case T25_G3:
            memcpy_P(&desc, &desc_T25_G3, sizeof(InverterDescriptor));
            return desc;
        case V30:
            memcpy_P(&desc, &desc_V30, sizeof(InverterDescriptor));
            return desc;
        case V33:
            memcpy_P(&desc, &desc_V33, sizeof(InverterDescriptor));
            return desc;
        case V36:
            memcpy_P(&desc, &desc_V36, sizeof(InverterDescriptor));
            return desc;
        case V40:
            memcpy_P(&desc, &desc_V40, sizeof(InverterDescriptor));
            return desc;
        case V50:
            memcpy_P(&desc, &desc_V50, sizeof(InverterDescriptor));
            return desc;
        case V60:
            memcpy_P(&desc, &desc_V60, sizeof(InverterDescriptor));
            return desc;
        case V75:
            memcpy_P(&desc, &desc_V75, sizeof(InverterDescriptor));
            return desc;
        case VL15:
            memcpy_P(&desc, &desc_VL15, sizeof(InverterDescriptor));
            return desc;
        case VL20:
            memcpy_P(&desc, &desc_VL20, sizeof(InverterDescriptor));
            return desc;
        case VL25:
            memcpy_P(&desc, &desc_VL25, sizeof(InverterDescriptor));
            return desc;
        case VL30:
            memcpy_P(&desc, &desc_VL30, sizeof(InverterDescriptor));
            return desc;
        case VL37_5:
            memcpy_P(&desc, &desc_VL37_5, sizeof(InverterDescriptor));
            return desc;
        case R75:
            memcpy_P(&desc, &desc_R75, sizeof(InverterDescriptor));
            return desc;
        case R100:
            memcpy_P(&desc, &desc_R100, sizeof(InverterDescriptor));
            return desc;
        case R110:
            memcpy_P(&desc, &desc_R110, sizeof(InverterDescriptor));
            return desc;
        case GW3000_XS:
            memcpy_P(&desc, &desc_GW3000_XS, sizeof(InverterDescriptor));
            return desc;
        case GW3300_XS_30:
            memcpy_P(&desc, &desc_GW3300_XS_30, sizeof(InverterDescriptor));
            return desc;
        case GW5000_DNS:
            memcpy_P(&desc, &desc_GW5000_DNS, sizeof(InverterDescriptor));
            return desc;
        case GW5K_DNS_G40:
            memcpy_P(&desc, &desc_GW5K_DNS_G40, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_2KTL:
            memcpy_P(&desc, &desc_SUN2000_2KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_3KTL:
            memcpy_P(&desc, &desc_SUN2000_3KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_4KTL:
            memcpy_P(&desc, &desc_SUN2000_4KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_5KTL:
            memcpy_P(&desc, &desc_SUN2000_5KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_6KTL:
            memcpy_P(&desc, &desc_SUN2000_6KTL, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_12KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_12KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_12KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_12KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_15KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_15KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_15KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_15KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_20KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_20KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_20KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_20KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_30KTL:
            memcpy_P(&desc, &desc_SUN2000_33KTL_A, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_36KTL_A:
            memcpy_P(&desc, &desc_SUN2000_36KTL_A, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_30KTL_M3:
            memcpy_P(&desc, &desc_SUN2000_30KTL_M3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_36KTL_M3:
            memcpy_P(&desc, &desc_SUN2000_36KTL_M3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_40KTL_M3:
            memcpy_P(&desc, &desc_SUN2000_40KTL_M3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_60KTL_M0:
            memcpy_P(&desc, &desc_SUN2000_60KTL_M0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_75KTL_M1:
            memcpy_P(&desc, &desc_SUN2000_75KTL_M1, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_100KTL_M2:
            memcpy_P(&desc, &desc_SUN2000_100KTL_M2, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_100KTL_H1:
            memcpy_P(&desc, &desc_SUN2000_100KTL_H1, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_215KTL_H0:
            memcpy_P(&desc, &desc_SUN2000_215KTL_H0, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_215KTL_H3:
            memcpy_P(&desc, &desc_SUN2000_215KTL_H3, sizeof(InverterDescriptor));
            return desc;
        case SUN2000_250KTL_H1:
            memcpy_P(&desc, &desc_SUN2000_250KTL_H1, sizeof(InverterDescriptor));
            return desc;
        default:
            memset(&desc, 0, sizeof(InverterDescriptor));
            return desc;
    }
    memset(&desc, 0, sizeof(InverterDescriptor));
    return desc;
}