# NOTE
# Se a estrutura do projeto mudar, os includes feitos pelos arquivos devem alterar
# Feito isso, o script irá se adaptar à nova estrutura

import glob
import re

# modules_hash contem os modulos(nome -> var's)
modules_hash = {}
outputfile = open("output.txt","w")

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
        
def print_array(array):
    print('%s' % '\n'.join(map(str, array)))
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
        # Recebe o path do objeto relativo ao nome recebido(list_objevts)
        module_objects.append(modules_hash[i].path_obj) 
    
    compile_main = "g++ -std=c++11" + " -c " + main_path + " -o " +  app_path + ".o"
    # Compila o app linkando os .o's
    compile_app  = "g++ -o " + app_path + " " + app_path + ".o " + ' '.join(map(str,module_objects)) + module_flags    
    outputfile.write('\t' + compile_main + "\n")
    outputfile.write('\t' + compile_app  + "\n")
def construct_mains(main_paths):
    for main in main_paths:
        # Para cada main, busque todos seus includes que precisam ser linkados
        file = open(main)
        this_main_includes = search_includes(file.read())        
        recursive_includes_result = []
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

hpp_paths  = get_files_path("libbsn",".h","include")
hpp_paths += get_files_path("simulation",".hpp","include")
hpp_paths += get_files_path("module",".h","include")

cpp_paths  = get_files_path("libbsn",".cpp","src")
cpp_paths += get_files_path("simulation",".cpp","src")
cpp_paths += get_files_path("module",".cpp","src")
outputfile.write("compile:\n")
construct_classes(cpp_paths,hpp_paths)
compile_modules()
outputfile.write('\n')
main_paths  = get_files_path("module",".cpp","apps")
main_paths += get_files_path("simulation",".cpp","apps")
main_paths += get_files_path("libbsn",".cpp","apps")

construct_mains(main_paths)
