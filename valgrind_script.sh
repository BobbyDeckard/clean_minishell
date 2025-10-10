echo "Currently available options are:"
echo "	1. Builtins"
echo "	2. Pipes"
echo "	3. Redirs"
echo -n "Please select what you want to test: "
read option
echo -n "Do you want to check diff with bash ? (y/n): "
read diff_flag
if [ $option = "1" ]; then
	filename="test_builtins.txt"
elif [ $option = "2" ]; then
	filename="test_pipes.txt"
elif [ $option = "3" ]; then
	filename="test_redirs.txt"
fi
echo
echo
if [ $diff_flag = "y" ]; then
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --track-fds=yes --log-file=val_out.log --suppressions=ignore_readline_leaks.supp ./minishell < $filename > minishell_output.txt
	bash < $filename > bash_output.txt
	diff minishell_output.txt bash_output.txt > output.diff
else
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --track-fds=yes --log-file=val_out.log --suppressions=ignore_readline_leaks.supp ./minishell < $filename
fi