<?php

if (!isset($argv[1])) {
	die(sprintf('Usage: %s <path to GSINTERPRETER.dll>', $arv[0]));
}

if (!file_exists($argv[1])) {
	die(sprintf('%s does not exist!', $argv[1]));
}

// Get GSINTERPRETER directory.
$path = implode('\\', array_slice(explode('\\', $argv[1]), 0, -1)) . '\\';

// Create the def file.
$defFile = $path . 'GSINTERPRETER.def';
file_put_contents($defFile, "EXPORTS\n");

// Get array of exported names.
$output = shell_exec('dumpbin /exports "' . $argv[1] . '"');
$data = array_slice(explode("\n", $output), 19, -10);

// Output them to def file.
foreach ($data as $line) {
	file_put_contents($defFile, end(explode(' ', $line)) . "\n", FILE_APPEND);
}

// Now generate the lib.
$out = shell_exec(sprintf('lib /def:"%s" /OUT:"%s"', $defFile, $path . 'GSINTERPRETER.lib'));
var_dump($out);