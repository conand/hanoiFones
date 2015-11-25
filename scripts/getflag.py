import socket
import sys
import pexpect
import pexpect.fdpexpect
import re


def get_flag(ip, port, flag_id, token):
    conn = socket.create_connection((ip,port))
    c = pexpect.fdpexpect.fdspawn(conn.fileno())

    c.expect('\?:')
    c.sendline('2')
    c.expect('Insert the auction ID:')
    c.sendline(flag_id)
    c.expect('Insert the password:')
    c.sendline(token)
    c.expect('Your IMEI: \w+')
    flag = re.search('^Your IMEI: (\w+)$', c.after).group(1)
    c.expect('\?:')
    c.sendline('4')    
    c.close()
    
    conn.close()

    return {'FLAG': flag}


if __name__ == "__main__":
    print get_flag('127.0.0.1', '4000', sys.argv[1], sys.argv[2])
