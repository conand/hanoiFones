import pexpect
import pexpect.fdpexpect
import socket
import random
import sys
import re

MIN_BETS = 10
MAX_BETS = 20
MAX_INCREMENT = 15

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


def benign(ip, port):
    service = Service(ip, port)
    c = service.get()

    random_imei = ''.join(str(random.randint(0,9)) for x in xrange(15))
    nbets = random.randint(MIN_BETS, MAX_BETS)

    bets = []

    c.expect('\?:')
    c.sendline('1')
    c.expect('Insert the IMEI of the \(hano\)iFon:')
    c.sendline(random_imei)
    c.expect('Auction ID: \w+')
    auction_id = re.search('^Auction ID: (\w+)$', c.after).group(1)
    c.expect('Your Password: \w+')
    # password = re.search('^Your Password: (\w+)$', c.after).group(1)

    c.expect('\?:')
    c.sendline('3')
    c.expect('Insert the auction ID:')
    c.sendline(auction_id)

    options = [
        '1\) Make an offer',
        'Time is over!'
    ]

    offer_re = re.compile('^Best offer by now is (\d+)\$')
    for i in xrange(0, nbets):
        match = c.expect(options)
        if match != 0:
            break
        c.expect('\?:')
        c.sendline('1')
        c.expect('Best offer by now is \d+\$. You have to offer more!')
        cur = int(offer_re.search(c.after).group(1))
        bet = cur + random.randint(1, MAX_INCREMENT)
        bets.append(bet)
        c.expect('How much do you want to offer?')
        c.sendline(str(bet))

    options = [
        'Your last bets',
        '1\) Make an offer',
    ]
    match = c.expect(options)
    while match != 0:
        c.sendline('2')
        match = c.expect(options)

    bets_re = re.compile('^- (\d+)$')
    for b in bets[-1:]:
        c.expect('- (\d+)')
        if b != int(bets_re.search(c.after).group(1)):
            service.close()
            raise Exception('Bets mismatch!')

    c.expect('\?:')
    c.sendline('4')
    service.close()


if __name__ == "__main__":
    benign(None, None)
