#include "ManagerModule.hpp"

#define G3_T1_1X 0
#define G3_T1_2X 1
#define G3_T1_3X 2
#define G3_T1_4X 3

using namespace odcore::base::module;
using namespace odcore::data;

using namespace bsn::goalmodel;
using namespace bsn::msg::info; 
using namespace bsn::msg::control;

ManagerModule::ManagerModule(const int32_t  &argc, char **argv) :
    TimeTriggeredConferenceClientModule(argc, argv, "manager"),
    buffer(),

    tasks(),
    contexts(),

    cost_expression(),
    reliability_expression(),

    cost_formula_reliabilities(),
    cost_formula_frequencies(),
    cost_formula_costs(),
    cost_formula_contexts(),

    reliability_formula_reliabilities(),
    reliability_formula_frequencies(),
    reliability_formula_contexts(),

    actions(),
    strategies() {}

ManagerModule::~ManagerModule() {}

void ManagerModule::setUp() {
    addDataStoreFor(700, buffer);
    addDataStoreFor(701, buffer);
    addDataStoreFor(702, buffer);

    { // Set up map {id,object} of leaf task from goal model
        // Pulse oximeter
        tasks.insert(std::pair<std::string,Task>("G3_T1.11",Task("G3_T1.11","Read data","W_G3_T1_11","R_G3_T1_11","F_G3_T1_11")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.12",Task("G3_T1.12","Filter data","W_G3_T1_12","R_G3_T1_12","F_G3_T1_12")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.13",Task("G3_T1.13","Transfer data","W_G3_T1_13","R_G3_T1_13","F_G3_T1_13")));

        // Electrocardiograph
        tasks.insert(std::pair<std::string,Task>("G3_T1.21",Task("G3_T1.21","Read data","W_G3_T1_21","R_G3_T1_21","F_G3_T1_21")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.22",Task("G3_T1.22","Filter data","W_G3_T1_22","R_G3_T1_22","F_G3_T1_22")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.23",Task("G3_T1.23","Transfer data","W_G3_T1_23","R_G3_T1_23","F_G3_T1_23")));

        // Thermometer
        tasks.insert(std::pair<std::string,Task>("G3_T1.31",Task("G3_T1.31","Read data","W_G3_T1_31","R_G3_T1_31","F_G3_T1_31")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.32",Task("G3_T1.32","Filter data","W_G3_T1_32","R_G3_T1_32","F_G3_T1_32")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.33",Task("G3_T1.33","Transfer data","W_G3_T1_33","R_G3_T1_33","F_G3_T1_33")));

        // Sphygmomanometer
        tasks.insert(std::pair<std::string,Task>("G3_T1.411",Task("G3_T1.411","Read diastolic","W_G3_T1_411","R_G3_T1_411","F_G3_T1_411")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.412",Task("G3_T1.412","Read systolic","W_G3_T1_412","R_G3_T1_412","F_G3_T1_412")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.42",Task("G3_T1.42","Filter data","W_G3_T1_42","R_G3_T1_42","F_G3_T1_42")));
        tasks.insert(std::pair<std::string,Task>("G3_T1.43",Task("G3_T1.43","Transfer data","W_G3_T1_43","R_G3_T1_43","F_G3_T1_43")));

        // Centralhub
        tasks.insert(std::pair<std::string,Task>("G4_T2.1",Task("G4_T2.1","Fuse sensor data","W_G4_T1_1","R_G4_T1_1","F_G4_T1_1")));
        tasks.insert(std::pair<std::string,Task>("G4_T2.2",Task("G4_T2.2","Detect patient status","W_G4_T1_2","R_G4_T1_2","F_G4_T1_2")));
        tasks.insert(std::pair<std::string,Task>("G4_T2.3",Task("G4_T2.3","Persist data","W_G4_T1_3","R_G4_T1_3","F_G4_T1_3")));
    }

    { // Set up map {id,object} of context from goal model
        contexts.insert(std::pair<std::string,Context>("SaO2_available",Context("SaO2_available",false,"CTX_G3_T1_1")));
        contexts.insert(std::pair<std::string,Context>("ECG_available",Context("ECG_available",false,"CTX_G3_T1_2")));
        contexts.insert(std::pair<std::string,Context>("TEMP_available",Context("TEMP_available",false,"CTX_G3_T1_3")));
        contexts.insert(std::pair<std::string,Context>("ABP_available",Context("ABP_available",false,"CTX_G3_T1_4")));
    }

    { // Set up cost and reliability expressions
        std::ifstream cost_file;
        std::string cost_formula;

        std::ifstream reliability_file;
        std::string reliability_formula;

        try{
            cost_file.open("../formulae/cost.formula");
            std::getline(cost_file,cost_formula);
            cost_file.close();
        } catch (std::ifstream::failure e) { std::cerr << "Exception opening/reading/closing file (cost.formula)\n"; }

        try{
            reliability_file.open("../formulae/reliability.formula");
            std::getline(reliability_file,reliability_formula);
            reliability_file.close();
        } catch (std::ifstream::failure e) { std::cerr << "Exception opening/reading/closing file (reliability.formula)\n"; }
    
        cost_expression = Lepton::Parser::parse(cost_formula).createCompiledExpression();
        reliability_expression = Lepton::Parser::parse(reliability_formula).createCompiledExpression();

        for (std::pair<std::string,Task> task : tasks){
            cost_formula_reliabilities          .insert(std::pair<std::string,double&>(task.second.getTask(),cost_expression.getVariableReference(task.second.getReliabilitySymbol())));
            cost_formula_frequencies            .insert(std::pair<std::string,double&>(task.second.getTask(),cost_expression.getVariableReference(task.second.getFrequencySymbol())));
            cost_formula_costs                  .insert(std::pair<std::string,double&>(task.second.getTask(),cost_expression.getVariableReference(task.second.getCostSymbol())));
            
            reliability_formula_reliabilities   .insert(std::pair<std::string,double&>(task.second.getTask(),reliability_expression.getVariableReference(task.second.getReliabilitySymbol())));
            reliability_formula_frequencies     .insert(std::pair<std::string,double&>(task.second.getTask(),reliability_expression.getVariableReference(task.second.getFrequencySymbol())));
        }

        
        for (std::pair<std::string,Context> context : contexts) {
            cost_formula_contexts               .insert(std::pair<std::string,double&>(context.second.getContext(),cost_expression.getVariableReference(context.second.getContextSymbol())));
            reliability_formula_contexts        .insert(std::pair<std::string,double&>(context.second.getContext(),reliability_expression.getVariableReference(context.second.getContextSymbol())));
        }
    }

    { // Set up actions
        
        actions = std::vector<std::vector<double>> {
                                        {0.90,0.95,1.00},
                                        {0.90,0.95,1.00},
                                        {0.90,0.95,1.00},
                                        {0.90,0.95,1.00}};

        for (int idx = 0, w = 0, x = 0, y = 0, z = 0; idx < std::pow(actions[0].size(),actions.size()); ++idx){
            strategies.push_back({(double)w, (double)x, (double)y, (double)z});

            if(++z == 3) { 
                z = 0;
                if(++y == 3) { 
                    y = 0;
                    if(++x == 3) { 
                        x = 0;
                        if(++w == 3) { 
                            w = 0;
                        }
                    }
                }
            }
        }

        for (std::vector<std::vector<double>>::iterator it = strategies.begin(); it != strategies.end(); ++it) {
            for (std::vector<double>::iterator itt = (*it).begin(); itt != (*it).end(); ++itt) {
                if ((int)*itt == 0) *itt = 0.9;
                else if ((int)*itt == 1) *itt = 0.95;
                else if ((int)*itt == 2) *itt = 1;
            }
        }
    }

}

void ManagerModule::tearDown(){}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode ManagerModule::body(){

    double cost;
    double reliability;
    std::string patient_health_status = "NORMAL STATE";
    double cost_goal;
    double reliability_goal;
    bool new_info = false;

    while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

        while(!buffer.isEmpty()){

            Container container = buffer.leave();

            if(container.getDataType() == 700) { // update task properties

                std::string task_id = container.getData<TaskInfo>().getTask();
                double task_cost = container.getData<TaskInfo>().getCost();
                double task_reliability = container.getData<TaskInfo>().getReliability();
                double task_frequency = container.getData<TaskInfo>().getFrequency();

                tasks[task_id].setCost(task_cost);
                tasks[task_id].setReliability(task_reliability);
                tasks[task_id].setFrequency(task_frequency);

            } else if (container.getDataType() == 701) { // update context

                std::string context_id = container.getData<ContextInfo>().getContext();
                bool value = container.getData<ContextInfo>().getValue();

                contexts[context_id].setValue(value);

            } else if (container.getDataType() == 702) { // update patient health status
                patient_health_status = container.getData<PatientStatusInfo>().getPatientStatus();

                std::cout << "new patient status received> "<< patient_health_status << std::endl;
                if (patient_health_status == "CRITICAL STATE") {
                    cost_goal = 1;
                    reliability_goal = 0.99;
                } else {
                    cost_goal = 0.032;
                    reliability_goal = 0.85;
                }
            }

            new_info = true;
        }
        
        if (new_info) {
            new_info = false;
            { // plug in costs, reliabilities, frequencies and contexts and evaluate formulas
                { // in cost formula
                    for (std::pair<std::string,double&> cost_formula_reliability : cost_formula_reliabilities) {
                        cost_formula_reliability.second = tasks[cost_formula_reliability.first].getReliability();
                        //std::cout << tasks[cost_formula_reliability.first].getReliabilitySymbol() << ": " << cost_formula_reliability.second << std::endl;
                    }

                    for (std::pair<std::string,double&> cost_formula_frequency : cost_formula_frequencies) {
                        cost_formula_frequency.second = tasks[cost_formula_frequency.first].getFrequency();
                        //std::cout << tasks[cost_formula_frequency.first].getFrequencySymbol() << ": " << cost_formula_frequency.second << std::endl;
                    }

                    for (std::pair<std::string,double&> cost_formula_cost : cost_formula_costs) {
                        cost_formula_cost.second = tasks[cost_formula_cost.first].getCost();
                        //std::cout << tasks[cost_formula_cost.first].getCostSymbol() << ": " << cost_formula_cost.second << std::endl;
                    }

                    for (std::pair<std::string,double&> cost_formula_context : cost_formula_contexts) {
                        cost_formula_context.second = contexts[cost_formula_context.first].getValue() ? 1:0;
                        //std::cout << contexts[cost_formula_context.first].getContextSymbol() << ": " << cost_formula_context.second << std::endl;
                    }

                    cost = cost_expression.evaluate();
                }

                { // in reliability formula

                    for (std::pair<std::string,double&> reliability_formula_reliability : reliability_formula_reliabilities) {
                        reliability_formula_reliability.second = tasks[reliability_formula_reliability.first].getReliability();
                    }

                    for (std::pair<std::string,double&> reliability_formula_frequency : reliability_formula_frequencies) {
                        reliability_formula_frequency.second = tasks[reliability_formula_frequency.first].getFrequency();
                    }

                    for (std::pair<std::string,double&> reliability_formula_context : reliability_formula_contexts) {
                        reliability_formula_context.second = contexts[reliability_formula_context.first].getValue() ? 1:0;
                    }

                    reliability = reliability_expression.evaluate();
                }
            }

            std::cout << "--------------------------------------------------" << std::endl;
            std::cout << "|patient health status: " << patient_health_status << std::endl;
            std::cout << "|cost: " << cost << std::endl;
            std::cout << "|reliability: " << reliability << std::endl;
            std::cout << "--------------------------------------------------" << std::endl;

            if (reliability < reliability_goal || cost > cost_goal) { //triggers adaptation
                std::map<std::vector<double>, std::vector<double>> policies;

                for (std::vector<double> strategy : strategies ) { // substitutues each strategy the formulas and calculates cost and reliability
                    { // in cost formula
                        for (std::pair<std::string,double&> cost_formula_frequency : cost_formula_frequencies) {

                            if (cost_formula_frequency.first.find("G3_T1.1") != std::string::npos) {
                                cost_formula_frequency.second = strategy[0];
                            } else if (cost_formula_frequency.first.find("G3_T1.2") != std::string::npos) {
                                cost_formula_frequency.second = strategy[1];
                            } else if (cost_formula_frequency.first.find("G3_T1.3") != std::string::npos) {
                                cost_formula_frequency.second = strategy[2];
                            } else if (cost_formula_frequency.first.find("G3_T1.4") != std::string::npos) {
                                cost_formula_frequency.second = strategy[3];
                            }
                           
                        }

                        cost = cost_expression.evaluate();
                    }

                    { // in reliability formula
                        for (std::pair<std::string,double&> reliability_formula_frequency : reliability_formula_frequencies) {
                            if (reliability_formula_frequency.first.find("G3_T1.1") != std::string::npos) {
                                reliability_formula_frequency.second = strategy[0];
                            } else if (reliability_formula_frequency.first.find("G3_T1.2") != std::string::npos) {
                                reliability_formula_frequency.second = strategy[1];
                            } else if (reliability_formula_frequency.first.find("G3_T1.3") != std::string::npos) {
                                reliability_formula_frequency.second = strategy[2];
                            } else if (reliability_formula_frequency.first.find("G3_T1.4") != std::string::npos) {
                                reliability_formula_frequency.second = strategy[3];
                            }
                        }

                        reliability = reliability_expression.evaluate();
                    }
                    
                    policies[strategy] = {reliability,cost};            
                }

                for (std::pair<std::vector<double>,std::vector<double>> policy : policies) {

                    //std::cout << "[" << policy.first[0] << "," << policy.first[1] << "," << policy.first[2] << "," << policy.first[3] << "] ";
                    //std::cout << "--> reliability: " << policy.second[0] /*<< " cost: " << policy.second[1]*/ << std::endl;
                    if(policy.second[0] >= reliability_goal && policy.second[1] <= cost_goal) {
                        std::cout << "Sending message to sensors..." << std::endl;
                        
                        if (contexts["SaO2_available"].getValue()) {
                            OximeterControlCommand oControlCommand(contexts["SaO2_available"].getValue(), policy.first[0], 0);
                            Container oControlCommandContainer(oControlCommand);
                            getConference().send(oControlCommandContainer);
                        }

                        if (contexts["ECG_available"].getValue()) {
                            ECGControlCommand eControlCommand(contexts["ECG_available"].getValue(), policy.first[1], 0);
                            Container eControlCommandContainer(eControlCommand);
                            getConference().send(eControlCommandContainer);
                        }

                        if (contexts["TEMP_available"].getValue()) {
                            ThermometerControlCommand tControlCommand(contexts["TEMP_available"].getValue(), policy.first[2], 0);
                            Container tControlCommandContainer(tControlCommand);
                            getConference().send(tControlCommandContainer);
                        }

                        if (contexts["ABP_available"].getValue()) {
                            BloodpressureControlCommand sControlCommand(contexts["ABP_available"].getValue(), policy.first[3], 0);
                            Container sControlCommandContainer(sControlCommand);
                            getConference().send(sControlCommandContainer);
                        }

                        break;
                    }
                }
            }
        }
    }

    return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}
