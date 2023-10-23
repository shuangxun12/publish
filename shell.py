shell_builtin_cmd=[
    "cd",
    "help",
    "exit",
    "which"
]
def shell_builtin_nums():
    return len(shell_builtin_cmd)
def shell_cd(args):
    args.append(' ')
    if(args[1]==None):
        print('\033[31mMysh error at cd, lack of args\033[0m')
    else:
        import os,sys
        if(args[1]=='~'):
            path='/home'
        else:
           path=args[1]
        if os.path.isdir(path):
            os.chdir(path)
        else:
            print("\033[31mNo such dictory\033[0m")
    return 1
def shell_help(args):
    len1=shell_builtin_nums()
    print("This is shuangxun's shell")
    print("Here are some built in cmd:")
    for a in range(0,len1):
        print(shell_builtin_cmd[a])
    return 1
def shell_exit(args):
    return 0
def shell_which(args):
    args.append(' ')
    a=shell_builtin_nums()
    for i in range(0,a):
        if(args[1]==shell_builtin_cmd[i]):
            print('shell_'+shell_builtin_cmd[i])
            return 1
    import shutil 
    print(shutil.which(args[1]))
    return 1
builtin_func=[
    shell_cd,
    shell_help,
    shell_exit,
    shell_which
]
def shell_launch(args):
    import os
    line=' '.join(args)
    a=os.system(line)
    if(a!=0):
        print("\033[31mCouldn't find the commod\033[0m")
    return 1
def shell_exectue(args):
    a=shell_builtin_nums()
    if(args[0]=='') :
        return 1
    else:
        for i in range(0,a):
            if(args[0]==shell_builtin_cmd[i]):
                a=0
                return(builtin_func[i](args))
            else:
                a=1
    if(a==1):
        return shell_launch(args)
def shell_read_line(path):
    import datetime
    line=input()
    now=datetime.datetime.now()
    now=str(now)+':'
    file = open(path, 'a')
    file.write(now)
    file.write('\r\n')
    file.write(line)
    file.write('\r\n')
    file.close()
    line=line.split(" ")
    return line
def shell_loop(path):
    import os
    import getpass
    status=1
    line=None
    agrs=None
    str(line)
    str(agrs)
    user=getpass.getuser()
    while(int(status)==1):
        path1=os.getcwd()
        print('%s@: %s$' %(user,path1))
        args=shell_read_line(path)
        status=shell_exectue(args)
import os
file=open('history.txt','a')
path=os.path.abspath('history.txt')
file.close
shell_loop(path)