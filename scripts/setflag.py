import random
import string
import socket
import sys
import pexpect
import pexpect.fdpexpect
import re

def set_flag(ip, port, flag):

    conn = socket.create_connection((ip,port))
    c = pexpect.fdpexpect.fdspawn(conn.fileno())

    c.expect('\?:')
    print c.after
    print c.sendline('1')
    c.expect('Insert the IMEI of the \(hano\)iFon:')
    print c.after
    c.sendline(flag)
    c.expect('Auction ID: \w+')
    flag_id = re.search('^Auction ID: (\w+)$', c.after).group(1)
    c.expect('Your Password: \w+')
    password = re.search('^Your Password: (\w+)$', c.after).group(1)
    c.expect('\?:')
    c.sendline('4')    
    c.close()
    
    conn.close()

    return {
            'FLAG_ID': flag_id,  # Unique id for each flag
            'TOKEN': password,   # benign (get_flag) will know this, exploits will not
            }


if __name__ == "__main__":
    print set_flag('127.0.0.1', '4000', "FLG_just_testing")
