import os

# ANSI color codes
GREEN = '\033[92m'
RESET = '\033[0m'

def compile_cpp_files_in_directory(directory):
    # Define the C++ compiler and flags
    CC = 'g++'
    CFLAGS = '--std=c++17'
    
    # Get current script directory
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Construct full path to target directory
    full_directory = os.path.join(script_dir, 'include/cpp17/test', directory)
    
    # Get all .cpp files in the directory
    cpp_files = [file for file in os.listdir(full_directory) if file.endswith('.cpp')]
    
    # Prepare compile command
    print(f"{GREEN}###########-Compiling {directory}...{RESET}")
    for i in range(0, len(cpp_files)):
        compile_command = f"{CC} {CFLAGS} {full_directory}/{cpp_files[i]} -o a.out"
        os.system(compile_command)        
        print(f"{GREEN} -Test [{cpp_files[i]}] successful.{RESET}")
    os.system(f"rm a.out")
    print(f"{GREEN}###########-Compilation completed for {directory}.{RESET}\n")
    

def compile_all_test_directories():
    test_dirs = [
        'allocator_test',
        'container_test',
        'impl_test',
        'iterator_test',
        'util_test',
    ]
    
    # Compile each test directory
    for test_dir in test_dirs:
        compile_cpp_files_in_directory(test_dir)

if __name__ == "__main__":
    compile_all_test_directories()