import socket
import sys
import pexpect
import pexpect.fdpexpect
import re

class Service:
    def __init__(self, ip, port):
        if ip:
            self._conn = socket.create_connection((ip,port))
            self.child = pexpect.fdpexpect.fdspawn(self._conn.fileno())
            self.child.logfile = sys.stdout
        else:
            self._conn = None
            self.child = pexpect.spawn("../service/ro/hanoiFones")
            self.child.logfile = sys.stdout

    def get(self):
        return self.child

    def close(self):
        self.child.close()
        if self._conn:
            self._conn.close()


def get_flag(ip, port, flag_id, token):
    service = Service(ip, port)
    c = service.get()

    c.expect('\?:')
    c.sendline('2')
    c.expect('Insert the auction ID:')
    c.sendline(flag_id)
    options = [
        'Insert the password:',
        'Auction does not exists'
    ]
    match = c.expect(options)
    if match != 0:
        service.close()
        raise Exception(options[match])
    
    c.sendline(token)
    options = [
        'Your IMEI: \w+',
        'Wrong password!'
    ]
    match = c.expect(options)
    if match != 0:
        service.close()
        raise Exception(options[match])
    flag = re.search('^Your IMEI: (\w+)$', c.after).group(1)
    c.expect('\?:')
    c.sendline('4')    
    
    service.close()

    return {'FLAG': flag}


if __name__ == "__main__":
    print get_flag(None, None, sys.argv[1], sys.argv[2])
