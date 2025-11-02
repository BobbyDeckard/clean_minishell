echo "Currently available options are:"
echo "	1. Builtins"
echo "	2. Pipes"
echo "	3. Redirs"
echo "	4. Env vars"
echo "	5. Quotes"
echo "	6. Syntax"
echo "	7. Parsing"
echo "	8. Mix"
echo "	9. New tests"
echo -n "Please select what you want to test: "
read option
echo -n "Do you want to check diff with bash ? (y/n): "
read diff_flag
filename=tests/test_
if [ $option = "1" ]; then
	filename+=builtins.txt
	test=builtins
elif [ $option = "2" ]; then
	filename+=pipes.txt
	test=pipes
elif [ $option = "3" ]; then
	filename+=redirs.txt
	test=redirs
elif [ $option = "4" ]; then
	filename+=env_vars.txt
	test=env_vars
elif [ $option = "5" ]; then
	filename+=quotes.txt
	test=quotes
elif [ $option = "6" ]; then
	filename+=syntax.txt
	test=syntax
elif [ $option = "7" ]; then
	filename+=parsing.txt
	test=parsing
elif [ $option = "8" ]; then
	filename+=mix.txt
	test=mix
elif [ $option = "9" ]; then
	filename+=new_tests.txt
	test=new
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
