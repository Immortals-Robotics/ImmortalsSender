using namespace Immortals;

RF24 radio(22, 0);

int  Channel             = 110;
bool firstPacketRecieved = false;

uint8_t address[5] = {110, 110, 8, 110, 110};

void demo()
{
    uint8_t data[11];
    data[0]  = 25;
    data[1]  = 1;
    data[2]  = 0;
    data[3]  = 0;
    data[4]  = 0;
    data[5]  = 0;
    data[6]  = 0;
    data[7]  = 0;
    data[8]  = 0;
    data[9]  = 0;
    data[10] = 0;

    address[2] = data[0];
    radio.openWritingPipe(address);
    radio.write(data + 1, 10);
}

void processRecievedPacket(std::span<char> packet)
{
    firstPacketRecieved = true;

    unsigned head = 0;
    while (head < packet.size())
    {
        if (packet[head] == 80)
        {
            if (packet[head + 1] == packet[head + 7] && Channel != packet[head + 1])
            {
                Channel = packet[head + 1];
                Common::logInfo("setting nrf channel to {}", Channel);
                radio.setChannel(Channel);
                head += 10;
            }
        }
        else
        {
            address[2] = packet[head];
            radio.openWritingPipe(address);

            const int packet_len = packet[head + 1];
            radio.write(packet.data() + head + 2, packet_len);

            head += packet_len + 2;
        }
    }
}

int main(void)
{
    // initialize nrf
    if (!radio.begin())
    {
        Common::logCritical("nrf24 is not responding");
        return -1;
    }

    // TODO: verify
    radio.setPayloadSize(10);

    radio.setPALevel(RF24_PA_MAX);

    address[2] = 8;
    radio.openWritingPipe(address);

    address[2] = 30;
    radio.openReadingPipe(1, address);

    radio.setChannel(Channel);

    // TODO: verify
    radio.setAutoAck(false);

    // put radio in TX mode
    radio.stopListening();

    radio.printPrettyDetails();

    std::unique_ptr<Common::UdpClient> udp_client =
        std::make_unique<Common::UdpClient>(Common::NetworkAddress{"224.5.92.5", 60005});

    while (true)
    {
        std::span<char> packet{};
        if (udp_client->receiveRaw(&packet))
        {
            Common::logDebug("received {} bytes from {}", packet.size(), udp_client->getLastReceiveEndpoint());
            processRecievedPacket(packet);
        }

        if (!firstPacketRecieved)
        {
            Common::logDebug("sending demo bytes");
            demo();
        }
    }
}
