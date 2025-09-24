#!/usr/bin/python

from mininet.net import Mininet
from mininet.node import Controller, RemoteController, OVSController
from mininet.node import CPULimitedHost, Host, Node
from mininet.node import OVSSwitch, UserSwitch
from mininet.node import IVSSwitch
from mininet.cli import CLI
from mininet.log import setLogLevel, info
from mininet.link import TCLink, Intf
from subprocess import call

class InbandController( RemoteController ):

    def checkListening( self ):
        "Overridden to do nothing."
        return

def myNetwork():

    net = Mininet( topo=None,
                   build=False,
                   autoStaticArp = False)

    info( '*** Adding controller\n' )
    c0=net.addController(name='c0',
                      controller=RemoteController,
                      ip='0.0.0.0',
                      protocol='tcp',
                      port=6653)

    info( '*** Add switches\n')
    s1 = net.addSwitch('s1', cls=OVSSwitch, protocols='OpenFlow13')

    info( '*** Add hosts\n')
    h1 = net.addHost('h1', ip='10.0.0.1')
    h2 = net.addHost('h2', ip='10.0.0.2')


    # Add in link options for different experiments
    #linkopts = dict(delay='10ms', bw=1000)

    info( '*** Add links\n')
    net.addLink(s1, h1)
    net.addLink(s1, h2)

    info( '*** Starting network\n')
    net.build()
    info( '*** Starting controllers\n')
    c0.start()

    info( '*** Starting switches\n')
    net.get('s1').start([c0])


    info( '*** Post configure switches and hosts\n')
    s1.cmd('ifconfig s1 10.0.0.21 up')


    CLI(net)
    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    myNetwork()

