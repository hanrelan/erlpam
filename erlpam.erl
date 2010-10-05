-module(erlpam).
-export([init/0, auth_user/2]).

init() ->
	erlang:load_nif("./erlpam", 0).

auth_user(_Username, _Password)->
	false.
