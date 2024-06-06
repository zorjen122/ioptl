import os
import sys
import shutil

Vflag = "-Wall -Werror -Wextra --pedantic --std=c++20 -Wno-unused -Wno-error=deprecated-copy"
src_dir = "./src/cpp17/test/container_test/"
target_dir = "./build/bin/"

def excu_test(move_flag = True):
    for file in os.listdir(src_dir):
        if file.endswith(".cpp"):
            obj_file = f"{file[:-4]}.o"
            complie_cmd = f"g++ {Vflag} -c {src_dir}/{file} -o {obj_file}"
            test_cmd = f"g++ {Vflag} {src_dir}/{file} -o {file[:-4]}"
            exe_file = f"{file[:-4]}"
        
          #excu: 
            os.system(complie_cmd)
            os.system(test_cmd)
            os.remove(obj_file)
            if move_flag:
                os.remove(exe_file)
            else:
                if not os.path.exists():
                    os.mkdir(target_dir)
                shutil.move(exe_file,os.path.join(target_dir,exe_file))

    print("Compilation and testing complate")

if __name__ == "__main__":
    move_file = len(sys.argv) > 1 and sys.argv[1] == "w"
    excu_test(move_file);

