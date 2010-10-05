erlpam
======

erlpam is a simple Linux PAM authorization module for Erlang built using NIF.

You should change SERVICE_NAME the Makefile to something appropriate. 
You may also need to edit ERL_ROOT in the Makefile. 

To use the module, make sure erlpam.erl is in your load path, and erlpam.so is in your working directory.

Examples:
---------

	erl> erlpam:init().
	ok
	erl> erlpam:auth_user("some_user", "correct_password").
	true
	erl> erlpam:auth_user("some_user", "incorrect_password").
	false


Note: erlpam requires Erlang R14A or later



