import random
import string
import socket
import sys
import pexpect
import pexpect.fdpexpect


def set_flag(ip, port, flag):
    flag_id = "flag_id"
    password = "password"

    return {
            'FLAG_ID': flag_id,  # Unique id for each flag
            'TOKEN': password,   # benign (get_flag) will know this, exploits will not
            }


if __name__ == "__main__":
    print set_flag(None, None, "FLG_just_testing")
