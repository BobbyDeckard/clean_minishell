echo "Currently available options are:"
echo "	1. Builtins"
echo -n "Please select what you want to test: "
read option
if [ $option = "1" ]; then
	filename="test_builtins.txt"
fi
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --track-fds=yes --log-file=val_out.log --suppressions=ignore_readline_leaks.supp ./minishell < $filename > minishell_output.txt
bash < $filename > bash_output.txt
diff minishell_output.txt bash_output.txt > output.diff