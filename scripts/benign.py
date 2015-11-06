import random
import string
import socket
import pexpect
import pexpect.fdpexpect


def benign(ip, port):
    # There should be some other benign traffic to your service.
    # It helps determine if the service is functional or not, and makes it
    # harder to fingerprint flag-related traffic.

    pass

    # Nothing to return, if we got here without exceptions we assume that everything worked :)


if __name__ == "__main__":
    benign(None, None)
