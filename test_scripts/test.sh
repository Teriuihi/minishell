
myArray=("cd $HOME/Downloads" 
        "cd too_many_args args2"
        "cd ./path_not_found"
        "echo -n -n -nnnnn -nnnnn"
        "echo -n -nnn hello -n"
        "env | wc -l"
        "export env"
        )


for i in ${!myArray[@]}; do
    echo "${myArray[$i]}" >> results.txt
    ./minishell "${myArray[$i]}" >> results.txt
    printf "\n--------\n" >> results.txt
done
