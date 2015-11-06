import socket
import sys
import pexpect
import pexpect.fdpexpect


def get_flag(ip, port, flag_id, token):
    content = "test"

    return {'FLAG': content}


if __name__ == "__main__":
    print get_flag(None, None, sys.argv[1], sys.argv[2])
