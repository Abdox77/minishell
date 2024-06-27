int	stat_handler(int code, int flag)
{
	static int	exit_stat = 0;
	
	if (flag)
		exit_stat = code;
	return (exit_stat);
}
