# Este script foi feito para compilar módulos 
# Com a estrutura: src/(.cpp) e include/(.hpp)
# E mains em apps/(.cpp apenas)
# Alem disso, adicionalmente enxerga testes e instala folders desejados

# NOTE
# Se a estrutura do projeto mudar, os includes feitos pelos arquivos devem alterar
# Feito isso, o script irá se adaptar à nova estrutura

import glob
import os
import re

# modules_hash contem os modulos(nome -> var's)
modules_hash = {}
# outputfile contem o resultado do script
outputfile = open("Makefile","w")

class Module:
    # Variaveis:
    # includes(contem o nome de cada modulo incluido no programa)
    # nome do modulo path para: cpp,hpp e objeto
    def __init__(self,name,path_cpp,path_hpp,path_module,incs):
        self.name        = name
        self.path_src    = path_cpp
        self.path_inc    = path_hpp
        self.path_obj    = path_module + "build/" + self.name + ".o"
        self.includes    = incs
    def __str__(self):
        return (str(self.name) + '-' + str(self.path_src) + '-' + str(self.path_inc))
    def compile(self):
        # Se não possuir main, apenas compile o módulo(sem linkagem)
        obj = self.path_obj
        inc = self.path_inc[: self.path_inc.rfind('/')]
        src = self.path_src
        return "g++ -std=c++11 -I " + inc + " -c " + src + " -o " + obj
        
def print_array(arr):
    print(', '.join(arr))
def get_name(path):
    # A partir de um path para cpp ou hpp extrai o nome
    # Extrai o nome a partir da última ocorrencia de '/'
    name = path[path.rfind('/')+1:]
    name = name[:name.find('.')]
    return name

def get_module_path(path):    
    # O path extraído será usado para indicar aonde a build deve ir
    # Recebe um hpp ou cpp e extrai o path do modulo
    path = path[:path.find('.')]
    
    if "src/" in path:        
        path = path[:path.index("src/")]    
    elif "include/" in path:    
        path = path[:path.index("include/")]    
    elif "apps/" in path:    
        path = path[:path.index("apps/")]            

    return path

def search_includes(string):
    # Retorna um array com todos os includes de alguma file
    vet = re.findall(r'#include \"(.*?)\"', string)
    result = []
    for include in vet:
        # Append the names of the include files
        if "opendavinci" not in  include:
            result.append( get_name(include) )
    return result    

def get_files_path(folder,extension,especify_folder = ""):
    # Pega os paths das strings    
    result = glob.iglob(folder + '/**/*' + extension, recursive=True)

    if not especify_folder:
        # Caso não especifique um folder aonde deveria existir tal extensão
        # Apenas retorne tudo que foi pesquisado
        return result
    return_vet = []
    # Retorna apenas os paths com a pasta especificada
    for path in result:        
        if(especify_folder in path):
            return_vet.append(path)
    return return_vet         

def recursive_find_includes(this_include):
    array_includes = modules_hash[this_include].includes
    result = []
    if not array_includes:
        # Se n tiver includes -> base da recursao
        return this_include
    for include in array_includes:
        result.append(include)
        recursive_result = recursive_find_includes(include)
        if type(recursive_result) is str:
            result.append(recursive_result)
        else:    
            for r in recursive_result: 
                result.append(r)
    return result    
        
def construct_classes(cpp_paths, hpp_paths):
    for inc_path in hpp_paths:
        # Pega o nome
        name = get_name(inc_path)
        # Src relativo ao hpp 
        src_path = [s for s in cpp_paths if name in s]
        # e o path do modulo
        mod = get_module_path(inc_path)
        # pega as includes desse modulo
        file = open(inc_path)
        incs = search_includes(file.read())
        # Construindo assim uma instancia de modulo no hash
        modules_hash[name] = Module(name,src_path[0],inc_path,mod,incs)

def compile_modules():
    # Compila todas as instancias do hash
    for key, instance in modules_hash.items():
        outputfile.write('\t' + instance.compile() + "\n")

def compile_mains(list_objects,main_path,list_includes):
    # Passo da compilação envolvendo a linkagem
    module_name     = get_name(main_path)
    module_path     = get_module_path(main_path)    
    module_flags    = " -lopendavinci -lpthread"
    module_objects  = []
    module_includes = []
    app_path = module_path + "build/" + module_name
    for i in list_includes:
        # Recebe apenas o path ao inves dos nomes
        inc = modules_hash[i].path_inc
        inc = inc[:inc.rfind('/')]
        module_includes.append(inc)
    for i in list_objects:
        # Recebe o path do objeto relativo ao nome recebido(list_objects)
        module_objects.append(modules_hash[i].path_obj) 
    
    compile_main = "g++ -std=c++11" + " -c " + main_path + " -o " +  app_path + ".o"
    # Compila o app linkando os .o's
    compile_app  = "g++ -o " + app_path + " " + app_path + ".o " + ' '.join(map(str,module_objects)) + module_flags    
    outputfile.write('\t' + compile_main + "\n")
    outputfile.write('\t' + compile_app  + "\n")

def compile_tests(list_objects,path_test_h):
    path_to_test_module = path_test_h
    path_to_test_module = path_to_test_module[:path_to_test_module.rfind('/')]
    module_objects  = []
    module_flags    = " -lopendavinci -lpthread"

    for i in list_objects:
        # Recebe o path do objeto relativo ao nome recebido(list_objects)
        module_objects.append(modules_hash[i].path_obj) 

    path_runner = path_to_test_module + "/runner.cpp "
    path_test_object = path_to_test_module + "/a.o "
    path_test_main = path_to_test_module + "/main.out "
    outputfile.write('\t' + "@cxxtestgen --error-printer -o " + path_runner + path_test_h  + '\n')
    outputfile.write('\t' + "@g++ -std=c++11 -c " + path_runner + "-o " + path_test_object + '\n')
    outputfile.write('\t' + "@g++ -o " + path_test_main  + path_test_object + ' '.join(module_objects) + module_flags + '\n')
    outputfile.write('\t' + "./" + path_to_test_module + "/main.out" + '\n')

def construct_mains(main_paths):
    # Preparação dos arrays pra linkagem
    for main in main_paths:
        try:
            # Para cada main, busque todos seus includes que precisam ser linkados
            file = open(main)
            this_main_includes = search_includes(file.read())        
            recursive_includes_result = []
            if(get_name(main) in this_main_includes):
                this_main_includes.remove(get_name(main))
            for include in this_main_includes:  
                # Para cada include nessa main                               
                recursive_includes_result.append(include)
                recursion_result = recursive_find_includes(include)
                if type(recursion_result) is str:
                    recursive_includes_result.append(recursion_result)
                else:    
                    recursive_includes_result += recursion_result
            objects_list = (list(set(recursive_includes_result)))

            compile_mains(objects_list,main,this_main_includes)        
        except KeyError as exception:            
            print('On the file \"' + get_name(main) + '\" the include ' + (exception.args)[0] + ' doesn\'t exist')
            exit()
            #print('on ' + main + ' the inc ' + include)

def construct_tests(tests_path):
    # MÓDULO ADICIONAL(seu uso cabe ao usuário)
    # Esta função foi construída para a biblioteca cxx    
    for test in tests_path:
        
        try:
            file = open(test)
            
            this_main_includes = search_includes(file.read())        
            recursive_includes_result = []
            for include in this_main_includes:  
                # Para cada include no teste               
                recursive_includes_result.append(include)
                recursion_result = recursive_find_includes(include)
                if type(recursion_result) is str:
                    recursive_includes_result.append(recursion_result)
                else:    
                    recursive_includes_result += recursion_result
            objects_list = (list(set(recursive_includes_result)))
            # Seta a task pro teste
            outputfile.write(get_name(test) + ":\n")
            compile_tests(objects_list,test)        
        except KeyError as exception:            
            print('On the file \"' + get_name(test) + '\" the include ' + (exception.args)[0] + ' doesn\'t exist')
            exit()


def list_directories():
    # Lista todas as pastas abaixo desta
    # Pastas que começam com '.' serao ignoradas(como .git p.e.)
    result = []
    for i in next(os.walk('.'))[1]:
        if i[0] != '.':
            result.append(i)
    return result

def line_prepender(filename, line):
    # Coloca uma linha no começo do arquivo
    with open(filename, 'r+') as f:
        content = f.read()
        f.seek(0, 0)        
        f.write(line + '\n' + content)

def install(project_directory,project_name):
    # MÓDULO ADICIONAL(seu uso cabe ao usuário)
    # Instala uma pasta do projeto
    # Coloca os includes na pasta usr/local/include/<project_name>
    project_install_path = " /usr/local/include/" + project_name + '/'
    # Cria a pasta bsn 
    outputfile.write("\tsudo mkdir -p" + project_install_path + '\n')

    path_installs  = get_files_path(project_directory,".h","include")
    path_installs += get_files_path(project_directory,".hpp","include")
    for path in path_installs:
        final_path = path[path.index("include/") + len("include/"):]   
        if final_path and ("/" in final_path):
            # Se houver uma pasta a ser criada, crie-a
            final_path = final_path[:final_path.index("/")+1]            
            outputfile.write('\t' + "sudo mkdir -p " + project_install_path + final_path + '\n')

        install_command = "sudo cp " + path + project_install_path + final_path
        outputfile.write('\t' + install_command + '\n')
def uninstall(project_install_name):
    project_install_path = " /usr/local/include/" + project_install_name + '/'
    # Desinstala o projeto    
    outputfile.write("uninstall:\n")    
    outputfile.write("\tsudo rm -rf " + project_install_path)

# Compila tudo dentro dos diretorios e organiza a make por eles
current_directories = list_directories()
# Vetor make_all_tasks contem todas as tasks para o 'all' do make
make_all_tasks = []
# Vetor make_all_tasks contem o path de todas as mains
main_paths     = []
tests_paths    = []
backup_hash    = {}
for directory in current_directories:
    # Zera o hash para uma compilação limpa do módulo
    modules_hash = {}

    hpp_paths    = get_files_path(directory,".h","include")
    hpp_paths   += get_files_path(directory,".hpp","include")

    tests_paths += get_files_path(directory,".h","test")

    main_paths  += get_files_path(directory,".cpp","apps")

    cpp_paths    = get_files_path(directory,".cpp","src")
    #Compile o módulo apenas se houver algo para compilar(arrays não-vazios)
    if(cpp_paths and hpp_paths):        
        make_task_name = "compile-" + directory
        make_all_tasks.append(make_task_name)
        outputfile.write(make_task_name + ":\n")
        construct_classes(cpp_paths,hpp_paths)
        compile_modules()
        backup_hash = {**backup_hash,**modules_hash}

# Como são compilados os módulos de forma separada, 
# Uso um hash de backup pra salvar os dados de todos hashes
# De tal forma que todos juntos possuam informação para linkar as mains
modules_hash = backup_hash
outputfile.write("compile-main" + ":\n")
construct_mains(main_paths)
construct_tests(tests_paths)

#Instala projeto libbsn
outputfile.write("install:\n")
install("libbsn","bsn")
uninstall("bsn")

# Realiza o append do all e run_tests no inicio do arquivo
make_all_tasks = "all: " + " uninstall install " + ' '.join(make_all_tasks) + " compile-main" 
make_run_tests = "run_tests: " + ' '.join(map(get_name,tests_paths))
outputfile.close()
line_prepender("Makefile", make_all_tasks + '\n' + make_run_tests )