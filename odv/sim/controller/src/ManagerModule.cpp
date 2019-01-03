#include "ManagerModule.hpp"

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::goalmodel;
using namespace bsn::msg::info; 

ManagerModule::ManagerModule(const int32_t  &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "manager"),
    buffer(),
    tasks(),
    contexts(),
    cost_expression(),
    reliability_expression(),
    cost_formula_reliabilities(),
    cost_formula_costs(),
    cost_formula_contexts(),
    reliability_formula_reliabilities(),
    reliability_formula_contexts() {}

ManagerModule::~ManagerModule() {}

void ManagerModule::setUp() {
    addDataStoreFor(700, buffer);
    addDataStoreFor(701, buffer);

    { // Set up map {id,object} of leaf task from goal model
        // Pulse oximeter
        tasks.insert(std::pair<std::string,Task>("G3_T1.11",Task("G3_T1.11","Read data","W_G3_T1_11",0,"rTaskG3_T1_11",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.12",Task("G3_T1.12","Filter data","W_G3_T1_12",0,"rTaskG3_T1_12",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.13",Task("G3_T1.13","Transfer data","W_G3_T1_13",0,"rTaskG3_T1_13",0)));

        // Electrocardiograph
        tasks.insert(std::pair<std::string,Task>("G3_T1.21",Task("G3_T1.21","Read data","W_G3_T1_21",0,"rTaskG3_T1_21",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.22",Task("G3_T1.22","Filter data","W_G3_T1_22",0,"rTaskG3_T1_22",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.23",Task("G3_T1.23","Transfer data","W_G3_T1_23",0,"rTaskG3_T1_23",0)));

        // Thermometer
        tasks.insert(std::pair<std::string,Task>("G3_T1.31",Task("G3_T1.31","Read data","W_G3_T1_31",0,"rTaskG3_T1_31",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.32",Task("G3_T1.32","Filter data","W_G3_T1_32",0,"rTaskG3_T1_32",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.33",Task("G3_T1.33","Transfer data","W_G3_T1_33",0,"rTaskG3_T1_33",0)));

        // Sphygmomanometer
        tasks.insert(std::pair<std::string,Task>("G3_T1.411",Task("G3_T1.411","Read diastolic","W_G3_T1_411",0,"rTaskG3_T1_411",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.412",Task("G3_T1.412","Read systolic","W_G3_T1_412",0,"rTaskG3_T1_412",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.42",Task("G3_T1.42","Filter data","W_G3_T1_42",0,"rTaskG3_T1_42",0)));
        tasks.insert(std::pair<std::string,Task>("G3_T1.43",Task("G3_T1.43","Transfer data","W_G3_T1_43",0,"rTaskG3_T1_43",0)));

        // Centralhub
        tasks.insert(std::pair<std::string,Task>("G4_T2.1",Task("G4_T2.1","Fuse sensor data","W_G4_T1_1",0.8,"rTaskG4_T1_1",0.99)));
        tasks.insert(std::pair<std::string,Task>("G4_T2.2",Task("G4_T2.2","Detect patient status","W_G4_T1_2",0.8,"rTaskG4_T1_2",0.99)));
        tasks.insert(std::pair<std::string,Task>("G4_T2.3",Task("G4_T2.3","Persist data","W_G4_T1_3",0.8,"rTaskG4_T1_3",0.99)));
    }

    { // Set up map {id,object} of context from goal model
        contexts.insert(std::pair<std::string,Context>("SaO2_available",Context("SaO2_available",false,"CTX_G3_T1_1")));
        contexts.insert(std::pair<std::string,Context>("ECG_available",Context("ECG_available",false,"CTX_G3_T1_2")));
        contexts.insert(std::pair<std::string,Context>("TEMP_available",Context("TEMP_available",false,"CTX_G3_T1_3")));
        contexts.insert(std::pair<std::string,Context>("ABP_available",Context("ABP_available",false,"CTX_G3_T1_4")));
    }

    { // Set up cost and reliability expressions
        std::string cost_formula;
        std::string reliability_formula;
        std::ifstream cost_file;
        std::ifstream reliability_file;

        try{
            cost_file.open("../formulae/cost.out");
            std::getline(cost_file,cost_formula);
            cost_file.close();
        } catch (std::ifstream::failure e) { std::cerr << "Exception opening/reading/closing file (cost.out)\n"; }

        try{
            reliability_file.open("../formulae/reliability.out");
            std::getline(reliability_file,reliability_formula);
            reliability_file.close();
        } catch (std::ifstream::failure e) { std::cerr << "Exception opening/reading/closing file (reliability.out)\n"; }
    
        cost_expression = Lepton::Parser::parse(cost_formula).createCompiledExpression();
        reliability_expression = Lepton::Parser::parse(reliability_formula).createCompiledExpression();

        for (std::pair<std::string,Task> task : tasks){
            cost_formula_reliabilities.insert(std::pair<std::string,double&>(task.second.getTask(),cost_expression.getVariableReference(task.second.getReliabilitySymbol())));
            cost_formula_costs.insert(std::pair<std::string,double&>(task.second.getTask(),cost_expression.getVariableReference(task.second.getCostSymbol())));
            reliability_formula_reliabilities.insert(std::pair<std::string,double&>(task.second.getTask(),reliability_expression.getVariableReference(task.second.getReliabilitySymbol())));
        }

        
        for (std::pair<std::string,Context> context : contexts) {
            cost_formula_contexts.insert(std::pair<std::string,double&>(context.second.getContext(),cost_expression.getVariableReference(context.second.getContextSymbol())));
            reliability_formula_contexts.insert(std::pair<std::string,double&>(context.second.getContext(),reliability_expression.getVariableReference(context.second.getContextSymbol())));
        }
    }

}

void ManagerModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ManagerModule::body(){

    double cost;
    double reliability;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!buffer.isEmpty()){

            Container container = buffer.leave();

            if(container.getDataType() == 700) { // update task properties

                std::string task_id = container.getData<TaskInfo>().getTask();
                double task_cost = container.getData<TaskInfo>().getCost();
                double task_reliability = container.getData<TaskInfo>().getReliability();

                tasks[task_id].setCost(task_cost);
                tasks[task_id].setReliability(task_reliability);

            } else if (container.getDataType() == 701) { // update context

                std::string context_id = container.getData<ContextInfo>().getContext();
                bool value = container.getData<ContextInfo>().getValue();

                // context operationalization here ?

                contexts[context_id].setValue(value);

                // for debugging
                std::cout << "new context received: [" << context_id << ":" << value << "]" << std::endl;
            }

            std::cout << "--------------------------------------------------" << std::endl;
            // plug in costs, reliabilities and contexts...
            { // in cost formula and evaluate cost
                for (std::pair<std::string,double&> cost_formula_reliabiliy : cost_formula_reliabilities) {
                    cost_formula_reliabiliy.second = tasks[cost_formula_reliabiliy.first].getReliability();
                    std::cout << tasks[cost_formula_reliabiliy.first].getReliabilitySymbol() << ": " << cost_formula_reliabiliy.second << std::endl;
                }

                for (std::pair<std::string,double&> cost_formula_cost : cost_formula_costs) {
                    cost_formula_cost.second = tasks[cost_formula_cost.first].getCost();
                    std::cout << tasks[cost_formula_cost.first].getCostSymbol() << ": " << cost_formula_cost.second << std::endl;
                }

                for (std::pair<std::string,double&> cost_formula_context : cost_formula_contexts) {
                    cost_formula_context.second = contexts[cost_formula_context.first].getValue() ? 1:0;
                    std::cout << contexts[cost_formula_context.first].getContextSymbol() << ": " << cost_formula_context.second << std::endl;
                }

                cost = cost_expression.evaluate();
            }

            { // in reliability formula and evaluate reliability

                for (std::pair<std::string,double&> reliability_formula_reliability : reliability_formula_reliabilities) {
                    reliability_formula_reliability.second = tasks[reliability_formula_reliability.first].getReliability();
                }

                for (std::pair<std::string,double&> reliability_formula_context : reliability_formula_contexts) {
                    reliability_formula_context.second = contexts[reliability_formula_context.first].getValue() ? 1:0;
                }

                reliability = reliability_expression.evaluate();
            }

            std::cout << "cost: " << cost << std::endl;
            std::cout << "reliability: " << reliability << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

        }
        
        /*
        if (we need adaptation) {
            
        } 
        */
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
