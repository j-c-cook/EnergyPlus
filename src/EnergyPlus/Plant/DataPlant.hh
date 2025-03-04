// EnergyPlus, Copyright (c) 1996-2022, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef DataPlant_hh_INCLUDED
#define DataPlant_hh_INCLUDED

// C++ Headers
#include <numeric>

// ObjexxFCL Headers
#include <ObjexxFCL/Array1D.hh>
#include <ObjexxFCL/Fmath.hh>
#include <ObjexxFCL/Optional.hh>

// EnergyPlus Headers
#include <EnergyPlus/Data/BaseData.hh>
#include <EnergyPlus/DataGlobals.hh>
#include <EnergyPlus/DataLoopNode.hh>
#include <EnergyPlus/EnergyPlus.hh>
#include <EnergyPlus/OutputProcessor.hh>
#include <EnergyPlus/Plant/CallingOrder.hh>
#include <EnergyPlus/Plant/Enums.hh>
#include <EnergyPlus/Plant/Loop.hh>
#include <EnergyPlus/Plant/PlantAvailManager.hh>
#include <EnergyPlus/Plant/ReportLoopData.hh>

namespace EnergyPlus {

namespace DataPlant {

    // Using/Aliasing
    using DataLoopNode::SensedNodeFlagValue;

    // Criteria percentage limits for determining re-simulation of connected loop sides
    constexpr Real64 CriteriaDelta_MassFlowRate(0.001);
    constexpr Real64 CriteriaDelta_Temperature(0.010);
    constexpr Real64 CriteriaDelta_HeatTransferRate(0.100);

    // Parameters for tolerance
    constexpr Real64 LoopDemandTol(0.1);   // minimum significant loop cooling or heating demand
    constexpr Real64 DeltaTempTol(0.0001); // minimum significant loop temperature difference

    // Parameters for Component/Equipment Types  (ref: TypeOf in CompData)

    static constexpr std::array<std::string_view, static_cast<int>(PlantEquipmentType::Num)> PlantEquipTypeNames{
        "Boiler:HotWater",
        "Boiler:Steam",
        "Chiller:Absorption",
        "Chiller:Absorption:Indirect",
        "Chiller:CombustionTurbine",
        "Chiller:ConstantCOP",
        "ChillerHeater:Absorption:DirectFired",
        "Chiller:Electric",
        "Chiller:Electric:EIR",
        "Chiller:Electric:ReformulatedEIR",
        "Chiller:EngineDriven",
        "CoolingTower:SingleSpeed",
        "CoolingTower:TwoSpeed",
        "CoolingTower:VariableSpeed",
        "Generator:Fuelcell:ExhaustGastoWaterHeatExchanger",
        "WaterHeater:HeatPump:PumpedCondenser",
        "Heatpump:WatertoWater:Equationfit:Cooling",
        "Heatpump:WatertoWater:Equationfit:Heating",
        "Heatpump:WatertoWater:ParameterEstimation:Cooling",
        "Heatpump:WatertoWater:ParameterEstimation:Heating",
        "Pipe:Adiabatic",
        "Pipe:Adiabatic:Steam",
        "Pipe:Outdoor",
        "Pipe:Indoor",
        "Pipe:Underground",
        "DistrictCooling",
        "DistrictHeating",
        "ThermalStorage:Ice:Detailed",
        "ThermalStorage:Ice:Simple",
        "TemperingValve",
        "WaterHeater:Mixed",
        "WaterHeater:Stratified",
        "Pump:VariableSpeed",
        "Pump:ConstantSpeed",
        "Pump:VariableSpeed:Condensate",
        "HeaderedPumps:VariableSpeed",
        "HeaderedPumps:ConstantSpeed",
        "WaterUse:Connections",
        "Coil:Cooling:Water",
        "Coil:Cooling:Water:DetailedGeometry",
        "Coil:Heating:Water",
        "Coil:Heating:Steam",
        "Solarcollector:Flatplate:Water",
        "LoadProfile:Plant",
        "GroundHeatExchanger:System",
        "GroundHeatExchanger:Surface",
        "GroundHeatExchanger:Pond",
        "Generator:Microturbine",
        "Generator:InternalCombustionEngine",
        "Generator:CombustionTurbine",
        "Generator:Microchp",
        "Generator:Fuelcell:StackCooler",
        "FluidCooler:SingleSpeed",
        "FluidCooler:TwoSpeed",
        "EvaporativeFluidCooler:SingleSpeed",
        "EvaporativeFluidCooler:TwoSpeed",
        "ThermalStorage:ChilledWater:Mixed",
        "ThermalStorage:ChilledWater:Stratified",
        "SolarCollector:FlatPlate:PhotovoltaicThermal",
        "ZoneHVAC:Baseboard:Convective:Water",
        "ZoneHVAC:Baseboard:RadiantConvective:Steam",
        "ZoneHVAC:Baseboard:RadiantConvective:Water",
        "ZoneHVAC:LowTemperatureRadiant:VariableFlow",
        "ZoneHVAC:LowTemperatureRadiant:ConstantFlow",
        "AirTerminal:SingleDuct:ConstantVolume:CooledBeam",
        "Coil:Heating:WaterToAirHeatPump:EquationFit",
        "Coil:Cooling:WaterToAirHeatPump:EquationFit",
        "Coil:Heating:WaterToAirHeatPump:ParameterEstimation",
        "Coil:Cooling:WaterToAirHeatPump:ParameterEstimation",
        "Refrigeration:Condenser:WaterCooled",
        "Refrigeration:CompressorRack",
        "AirLoopHVAC:UnitaryHeatPump:AirToAir:MultiSpeed",
        "ChillerHeater:Absorption:DoubleEffect",
        "PipingSystem:Underground:PipeCircuit",
        "SolarCollector:IntegralCollectorStorage",
        "Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit",
        "Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit",
        "PlantComponent:UserDefined",
        "Coil:UserDefined",
        "ZoneHVAC:ForcedAir:UserDefined",
        "AirTerminal:SingleDuct:UserDefined",
        "AirConditioner:VariableRefrigerantFlow",
        "GroundHeatExchanger:HorizontalTrench",
        "HeatExchanger:FluidToFluid",
        "PlantComponent:TemperatureSource",
        "CentralHeatPumpSystem",
        "AirLoopHVAC:UnitarySystem",
        "Coil:Cooling:DX:SingleSpeed:ThermalStorage",
        "CoolingTower:VariableSpeed:Merkel",
        "SwimmingPool:Indoor",
        "GroundHeatExchanger:Slinky",
        "WaterHeater:HeatPump:WrappedCondenser",
        "AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam",
        "ZoneHVAC:CoolingPanel:RadiantConvective:Water",
        "HeatPump:PlantLoop:EIR:Cooling",
        "HeatPump:PlantLoop:EIR:Heating"};

    static constexpr std::array<std::string_view, static_cast<size_t>(PlantEquipmentType::Num)> PlantEquipTypeNamesUC{
        "BOILER:HOTWATER",
        "BOILER:STEAM",
        "CHILLER:ABSORPTION",
        "CHILLER:ABSORPTION:INDIRECT",
        "CHILLER:COMBUSTIONTURBINE",
        "CHILLER:CONSTANTCOP",
        "CHILLERHEATER:ABSORPTION:DIRECTFIRED",
        "CHILLER:ELECTRIC",
        "CHILLER:ELECTRIC:EIR",
        "CHILLER:ELECTRIC:REFORMULATEDEIR",
        "CHILLER:ENGINEDRIVEN",
        "COOLINGTOWER:SINGLESPEED",
        "COOLINGTOWER:TWOSPEED",
        "COOLINGTOWER:VARIABLESPEED",
        "GENERATOR:FUELCELL:EXHAUSTGASTOWATERHEATEXCHANGER",
        "WATERHEATER:HEATPUMP:PUMPEDCONDENSER",
        "HEATPUMP:WATERTOWATER:EQUATIONFIT:COOLING",
        "HEATPUMP:WATERTOWATER:EQUATIONFIT:HEATING",
        "HEATPUMP:WATERTOWATER:PARAMETERESTIMATION:COOLING",
        "HEATPUMP:WATERTOWATER:PARAMETERESTIMATION:HEATING",
        "PIPE:ADIABATIC",
        "PIPE:ADIABATIC:STEAM",
        "PIPE:OUTDOOR",
        "PIPE:INDOOR",
        "PIPE:UNDERGROUND",
        "DISTRICTCOOLING",
        "DISTRICTHEATING",
        "THERMALSTORAGE:ICE:DETAILED",
        "THERMALSTORAGE:ICE:SIMPLE",
        "TEMPERINGVALVE",
        "WATERHEATER:MIXED",
        "WATERHEATER:STRATIFIED",
        "PUMP:VARIABLESPEED",
        "PUMP:CONSTANTSPEED",
        "PUMP:VARIABLESPEED:CONDENSATE",
        "HEADEREDPUMPS:VARIABLESPEED",
        "HEADEREDPUMPS:CONSTANTSPEED",
        "WATERUSE:CONNECTIONS",
        "COIL:COOLING:WATER",
        "COIL:COOLING:WATER:DETAILEDGEOMETRY",
        "COIL:HEATING:WATER",
        "COIL:HEATING:STEAM",
        "SOLARCOLLECTOR:FLATPLATE:WATER",
        "LOADPROFILE:PLANT",
        "GROUNDHEATEXCHANGER:SYSTEM",
        "GROUNDHEATEXCHANGER:SURFACE",
        "GROUNDHEATEXCHANGER:POND",
        "GENERATOR:MICROTURBINE",
        "GENERATOR:INTERNALCOMBUSTIONENGINE",
        "GENERATOR:COMBUSTIONTURBINE",
        "GENERATOR:MICROCHP",
        "GENERATOR:FUELCELL:STACKCOOLER",
        "FLUIDCOOLER:SINGLESPEED",
        "FLUIDCOOLER:TWOSPEED",
        "EVAPORATIVEFLUIDCOOLER:SINGLESPEED",
        "EVAPORATIVEFLUIDCOOLER:TWOSPEED",
        "THERMALSTORAGE:CHILLEDWATER:MIXED",
        "THERMALSTORAGE:CHILLEDWATER:STRATIFIED",
        "SOLARCOLLECTOR:FLATPLATE:PHOTOVOLTAICTHERMAL",
        "ZONEHVAC:BASEBOARD:CONVECTIVE:WATER",
        "ZONEHVAC:BASEBOARD:RADIANTCONVECTIVE:STEAM",
        "ZONEHVAC:BASEBOARD:RADIANTCONVECTIVE:WATER",
        "ZONEHVAC:LOWTEMPERATURERADIANT:VARIABLEFLOW",
        "ZONEHVAC:LOWTEMPERATURERADIANT:CONSTANTFLOW",
        "AIRTERMINAL:SINGLEDUCT:CONSTANTVOLUME:COOLEDBEAM",
        "COIL:HEATING:WATERTOAIRHEATPUMP:EQUATIONFIT",
        "COIL:COOLING:WATERTOAIRHEATPUMP:EQUATIONFIT",
        "COIL:HEATING:WATERTOAIRHEATPUMP:PARAMETERESTIMATION",
        "COIL:COOLING:WATERTOAIRHEATPUMP:PARAMETERESTIMATION",
        "REFRIGERATION:CONDENSER:WATERCOOLED",
        "REFRIGERATION:COMPRESSORRACK",
        "AIRLOOPHVAC:UNITARYHEATPUMP:AIRTOAIR:MULTISPEED",
        "CHILLERHEATER:ABSORPTION:DOUBLEEFFECT",
        "PIPINGSYSTEM:UNDERGROUND:PIPECIRCUIT",
        "SOLARCOLLECTOR:INTEGRALCOLLECTORSTORAGE",
        "COIL:HEATING:WATERTOAIRHEATPUMP:VARIABLESPEEDEQUATIONFIT",
        "COIL:COOLING:WATERTOAIRHEATPUMP:VARIABLESPEEDEQUATIONFIT",
        "PLANTCOMPONENT:USERDEFINED",
        "COIL:USERDEFINED",
        "ZONEHVAC:FORCEDAIR:USERDEFINED",
        "AIRTERMINAL:SINGLEDUCT:USERDEFINED",
        "AIRCONDITIONER:VARIABLEREFRIGERANTFLOW",
        "GROUNDHEATEXCHANGER:HORIZONTALTRENCH",
        "HEATEXCHANGER:FLUIDTOFLUID",
        "PLANTCOMPONENT:TEMPERATURESOURCE",
        "CENTRALHEATPUMPSYSTEM",
        "AIRLOOPHVAC:UNITARYSYSTEM",
        "COIL:COOLING:DX:SINGLESPEED:THERMALSTORAGE",
        "COOLINGTOWER:VARIABLESPEED:MERKEL",
        "SWIMMINGPOOL:INDOOR",
        "GROUNDHEATEXCHANGER:SLINKY",
        "WATERHEATER:HEATPUMP:WRAPPEDCONDENSER",
        "AIRTERMINAL:SINGLEDUCT:CONSTANTVOLUME:FOURPIPEBEAM",
        "ZONEHVAC:COOLINGPANEL:RADIANTCONVECTIVE:WATER",
        "HEATPUMP:PLANTLOOP:EIR:COOLING",
        "HEATPUMP:PLANTLOOP:EIR:HEATING"};

    static constexpr std::array<LoopType, static_cast<size_t>(PlantEquipmentType::Num)> ValidLoopEquipTypes{
        LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant,
        LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant, LoopType::Plant,
        LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,
        LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant, LoopType::Plant, LoopType::Both,  LoopType::Both,  LoopType::Both,
        LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant, LoopType::Plant, LoopType::Plant,
        LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant,
        LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,
        LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Plant,
        LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant,
        LoopType::Plant, LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,
        LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant, LoopType::Plant, LoopType::Both,
        LoopType::Both,  LoopType::Both,  LoopType::Both,  LoopType::Plant, LoopType::Plant, LoopType::Plant, LoopType::Both,  LoopType::Both};

} // namespace DataPlant

struct DataPlantData : BaseGlobalStruct
{

    int TotNumLoops = 0;     // number of plant and condenser loops
    int TotNumHalfLoops = 0; // number of half loops (2 * TotNumLoops)
    bool PlantFirstSizeCompleted = false;
    bool PlantFirstSizesOkayToFinalize = false; // true if plant sizing is finishing and can save results
    bool PlantReSizingCompleted = false;
    bool PlantFirstSizesOkayToReport = false;
    bool PlantFinalSizesOkayToReport = false;
    bool AnyEMSPlantOpSchemesInModel = false;
    int PlantManageSubIterations = 0; // tracks plant iterations to characterize solver
    int PlantManageHalfLoopCalls = 0; // tracks number of half loop calls
    Array1D<DataPlant::PlantLoopData> PlantLoop;
    Array1D<DataPlant::PlantAvailMgrData> PlantAvailMgr;
    std::array<Array1D<DataPlant::ReportLoopData>, static_cast<int>(DataPlant::LoopSideLocation::Num)> VentRepPlant;
    std::array<Array1D<DataPlant::ReportLoopData>, static_cast<int>(DataPlant::LoopSideLocation::Num)> VentRepCond;
    Array1D<DataPlant::PlantCallingOrderInfoStruct> PlantCallingOrderInfo;

    void clear_state() override
    {
        this->TotNumLoops = 0;
        this->TotNumHalfLoops = 0;
        this->PlantFirstSizeCompleted = false;
        this->PlantFirstSizesOkayToFinalize = false;
        this->PlantReSizingCompleted = false;
        this->PlantFirstSizesOkayToReport = false;
        this->PlantFinalSizesOkayToReport = false;
        this->AnyEMSPlantOpSchemesInModel = false;
        this->PlantManageSubIterations = 0;
        this->PlantManageHalfLoopCalls = 0;
        this->PlantLoop.deallocate();
        this->PlantAvailMgr.deallocate();
        this->VentRepPlant[static_cast<int>(DataPlant::LoopSideLocation::Demand)].deallocate();
        this->VentRepPlant[static_cast<int>(DataPlant::LoopSideLocation::Supply)].deallocate();
        this->VentRepCond[static_cast<int>(DataPlant::LoopSideLocation::Demand)].deallocate();
        this->VentRepCond[static_cast<int>(DataPlant::LoopSideLocation::Supply)].deallocate();
        this->PlantCallingOrderInfo.deallocate();
    }
};

} // namespace EnergyPlus

#endif
