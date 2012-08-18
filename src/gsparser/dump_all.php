<?php
	
	/* Quick and dirty script to dump output of all GSClasses. */
	
	if(count($argv) < 2) {
		fprintf(STDERR, "Usage: php %s <class path>\n", $argv[0]);
		exit(-1);
	}
	
	if(!file_exists('parser.exe')) {
		fprintf(STDERR, "Please make sure parser.exe is present.\n");
		exit(-1);
	}
	
	if(!is_dir('dump')) {
		@mkdir('dump', 0777);
	}
	
	foreach(glob(rtrim($argv[1], '\\') . '\\*') as $current) {
		fprintf(STDOUT, "Dumping: %s\n", $current);
		$command = sprintf('parser.exe "%s" > "dump\%s.txt"', $current, end(explode('\\', $current)));
		shell_exec($command);
	}

?>