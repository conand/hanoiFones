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


def set_flag(ip, port, flag):
    service = Service(ip, port)
    c = service.get()

    c.expect('\?:')
    c.sendline('1')
    c.expect('Insert the IMEI of the \(hano\)iFon:')
    c.sendline(flag)
    c.expect('Auction ID: \w+')
    flag_id = re.search('^Auction ID: (\w+)$', c.after).group(1)
    c.expect('Your Password: \w+')
    password = re.search('^Your Password: (\w+)$', c.after).group(1)
    c.expect('\?:')
    c.sendline('4')

    service.close()

    return {
            'FLAG_ID': flag_id,  # Unique id for each flag
            'TOKEN': password,   # benign (get_flag) will know this, exploits will not
            }


if __name__ == "__main__":
    print set_flag(None, None, "FLG_just_testing")

