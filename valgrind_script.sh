echo "Currently available options are:"
echo "	1. Builtins"
echo "	2. Pipes"
echo "	3. Redirs"
echo "	4. Env vars"
echo "	5. Quotes"
echo "	6. Syntax"
echo -n "Please select what you want to test: "
read option
echo -n "Do you want to check diff with bash ? (y/n): "
read diff_flag
filename=tests/
if [ $option = "1" ]; then
	filename+=test_builtins.txt
	test=builtins
elif [ $option = "2" ]; then
	filename+=test_pipes.txt
	test=pipes
elif [ $option = "3" ]; then
	filename+=test_redirs.txt
	test=redirs
elif [ $option = "4" ]; then
	filename+=test_env_vars.txt
	test=env_vars
elif [ $option = "5" ]; then
	filename+=test_quotes.txt
	test=quotes
elif [ $option = "6" ]; then
	filename+=test_syntax.txt
	test=syntax
fi
echo
echo Starting tests.
echo
if [ $diff_flag = "y" ]; then
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --track-fds=yes --log-file=logs/diff_$test.log --suppressions=ignore_readline_leaks.supp ./minishell < $filename > logs/minishell_$test.out
	bash < $filename > logs/bash_$test.out
	diff logs/minishell_$test.out logs/bash_$test.out > logs/output_$test.diff
else
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --track-fds=yes --log-file=logs/$test.log --suppressions=ignore_readline_leaks.supp ./minishell < $filename
fi