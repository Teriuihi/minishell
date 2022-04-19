export var1="echo -n -n -nnn -mHI"
export var2="cd $HOME/Downloads"

array=(value1 ... "echo -n -n -nnn -mHI")
myArray=("cd $HOME/Downloads"
        "echo IT WORKS")


for i in ${!myArray[@]}; do
    "${myArray[$i]}"
    #echo $? >> results.txt
    printf "\n"
done
