#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include "erl_nif.h"

struct auth_info_t
{
	char user[256];
	char password[256];
};

static int conv(int num_msg, const struct pam_message **msg,
		struct pam_response** resp, void* appdata_ptr)
{
	struct auth_info_t* auth_info = appdata_ptr;

	int i;
	struct pam_response* reply;
	reply=malloc(sizeof(struct pam_response) * num_msg);
	for( i =0 ; i < num_msg; i++){
		switch(msg[i]->msg_style){
			case PAM_PROMPT_ECHO_ON: /* username */
				reply[i].resp = strdup(auth_info->user);
				reply[i].resp_retcode = PAM_SUCCESS;
				break;
			case PAM_PROMPT_ECHO_OFF: /*password*/
				reply[i].resp = strdup(auth_info->password);
				reply[i].resp_retcode = PAM_SUCCESS;
				break;
			default:
				printf("Unknown in conversation\n");
				free(reply);
				return PAM_CONV_ERR;

		}
	}
	*resp = reply;
	return PAM_SUCCESS;

}

int auth_user(const char *user, const char *password)
{
	pam_handle_t *pamh = NULL;
	int retval;
	int authenticated = 0;
	struct auth_info_t* auth_info = malloc(sizeof(struct auth_info_t));
	struct pam_conv pamc;
	strncpy(auth_info->user, user, 256);
	strncpy(auth_info->password, password, 256);
	pamc.conv = &conv;
	pamc.appdata_ptr = auth_info;
	
	retval = pam_start(SERVICE_NAME, user, &pamc, &pamh);

	if(retval == PAM_SUCCESS){
		retval = pam_authenticate(pamh, PAM_SILENT);
	}
	
	if(retval == PAM_SUCCESS){
		retval = pam_acct_mgmt(pamh, PAM_SILENT);
	}

	if(retval == PAM_SUCCESS){
		authenticated = 1;
	}
	else{
		authenticated = 0;
	}

	if(pam_end(pamh, retval) != PAM_SUCCESS) {
		pamh = NULL;
	}
	free(auth_info);
	return authenticated;

	
}

static ERL_NIF_TERM erl_auth_user(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
	char username[256];
	char password[256];
	enif_get_string(env, argv[0], username, 256, ERL_NIF_LATIN1);
	enif_get_string(env, argv[1], password, 256, ERL_NIF_LATIN1);
	int authenticated = auth_user(username, password);
	if(authenticated){
		return(enif_make_atom(env, "true"));
	}
	else{
		return(enif_make_atom(env, "false"));
	}

}

static ErlNifFunc nif_funcs[] =
{
	{"auth_user", 2, erl_auth_user}
};

ERL_NIF_INIT(erlpam, nif_funcs, NULL, NULL, NULL, NULL)
