using namespace Immortals;

#define CSN_PIN 0
#define CE_PIN 22

#define UDP_SERVER_PORT 60005 /* define the UDP local connection port */
#define UDP_CLIENT_PORT 60006 /* define the UDP remote connection port */

int checkFirst = 0;

RF24 radio(CE_PIN, CSN_PIN);

std::unique_ptr<Common::UdpClient> udp_client;

int fps = 0;

void setNumber(int a)
{
    (void) a;
}

void offSegment(int a)
{
    (void) a;
}

void beep(int delay)
{
    (void) delay;
}

void setLed(int i)
{
    (void) i;
}

int firstPacketRecieved = 0;

void demo()
{
    uint8_t       cc[20];
    unsigned char address[5] = {110, 110, 8, 110, 110};

    cc[0]  = 25;
    cc[1]  = 1;
    cc[2]  = 0;
    cc[3]  = 0;
    cc[4]  = 0;
    cc[5]  = 0;
    cc[6]  = 0;
    cc[7]  = 0;
    cc[8]  = 0;
    cc[9]  = 0;
    cc[10] = 0;

    address[2] = cc[0];
    radio.openWritingPipe(address);
    radio.write(cc + 1, 10);
}

int Channel = 110;

void Process_recieved_Packet(std::span<char> packet)
{
    fps++;
    firstPacketRecieved = 1;

    int     nextPacket = 0;
    uint8_t address[5] = {110, 110, 8, 110, 110};

    while (nextPacket < packet.size())
    {
        int packetLen = 0;
        if (packet[nextPacket] == 80)
        {
            if (packet[nextPacket + 1] == packet[nextPacket + 7] && Channel != packet[nextPacket + 1])
            {
                Channel = packet[nextPacket + 1];
                Common::logInfo("setting nrf channel to {}", Channel);
                radio.setChannel(Channel);
                beep(100);
                nextPacket += 10;
            }
        }
        else
        {
            address[2] = packet[nextPacket];
            packetLen  = packet[nextPacket + 1];
            radio.openWritingPipe(address);

            radio.write(packet.data() + nextPacket + 2, packetLen);

            if (checkFirst < 2)
                checkFirst++;

#if 0
            if(packet[nextPacket + 2]>127)
            {
                while(!nrf24l02_irq_pin_active());
                nrf24l02_irq_clear_tx_ds();
                nrf24l02_set_as_rx(true);
                delay_us(100);
                int timeOut = 400;
                while(timeOut > 0 && !nrf24l02_irq_pin_active())
                {
                    timeOut--;
                    delay_us(30);
                }
                if(timeOut>0 && nrf24l02_irq_rx_dr_active())
                {
                    unsigned char recData[10];

                    nrf24l02_read_rx_payload(recData,10);
                    ansBuf = pbuf_alloc(PBUF_TRANSPORT,10, PBUF_POOL);
                    IP4_ADDR(&boz, 224, 5 , 23, 3);
                    pbuf_take(ansBuf, (char*)recData, 10);
                    udp_sendto( upcb , ansBuf , &boz , UDP_CLIENT_PORT);
                    pbuf_free(ansBuf);
                }
                checkFirst=0;
                nrf24l02_irq_clear_rx_dr();
                nrf24l02_set_as_tx();
                delay_us(600);
            }
#endif

            nextPacket += packetLen + 2;
        }
    }
}

int main(void)
{
    uint8_t address[5] = {110, 110, 8, 110, 110};

    delay(500);

    for (int i = 0; i < 100; i++)
    {
        setNumber(i);
        delay(10);
    }
    beep(40);
    offSegment(3);
    delay(100);
    setNumber(0);
    delay(100);
    beep(40);
    offSegment(3);
    delay(100);
    setNumber(0);
    delay(100);
    beep(40);

    // perform hardware check
    if (!radio.begin())
    {
        Common::logCritical("nrf24 is not responding");
        return -1; // quit now
    }

    // TODO: verify
    radio.setPayloadSize(10);

    radio.setPALevel(RF24_PA_MAX);

    address[2] = 8;
    radio.openWritingPipe(address);

    address[2] = 30;
    radio.openReadingPipe(1, address);

    radio.setChannel(Channel);

    radio.setAutoAck(false);

    radio.stopListening(); // put radio in TX mode

    // For debugging info
    // radio.printDetails();       // (smaller) function that prints raw register values
    // radio.printPrettyDetails(); // (larger) function that prints human readable data

    udp_client = std::make_unique<Common::UdpClient>(Common::NetworkAddress{"224.5.92.5", UDP_SERVER_PORT});

    /* Infinite loop */
    while (1)
    {
        /* check if any packet received */
        std::span<char> packet{};
        if (udp_client->receiveRaw(&packet))
        {
            Common::logDebug("received {} bytes from {}", packet.size(), udp_client->getLastReceiveEndpoint());
            /* process received ethernet packet */
            Process_recieved_Packet(packet);
        }

        if (firstPacketRecieved == 0)
        {
            demo();
            setNumber(11);
        }
    }
}
