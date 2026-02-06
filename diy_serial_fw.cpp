// diy_serial_fw.cpp
// Minimal INDI filter wheel driver for your Nano serial firmware.
// libindi headers on your system: /usr/include/libindi/indifilterinterface.h + indifilterwheel.h
// Commands supported by firmware: ID, HOME, POS?, GOTO n
// Serial: 115200 8N1
//
// NOTE: This driver hardcodes /dev/ttyUSB0 for now (simple + reliable). We can add a port property later.

#include <libindi/indifilterwheel.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

#include <string>
#include <cctype>
#include <cstdlib>
#include <memory>

class DIYSerialFW : public INDI::FilterWheel
{
public:
    static constexpr int NUM_SLOTS = 5;

    DIYSerialFW() = default;
    ~DIYSerialFW() override { Disconnect(); }

    const char *getDefaultName() override
    {
        return "DIY Serial Filter Wheel";
    }

    bool initProperties() override
    {
        INDI::FilterWheel::initProperties();

        // Constrain the standard FilterSlot property to 1..NUM_SLOTS
        FilterSlotN[0].min  = 1;
        FilterSlotN[0].max  = NUM_SLOTS;
        FilterSlotN[0].step = 1;

        addDebugControl();
        return true;
    }

    bool Connect() override
    {
        const char *port = "/dev/ttyUSB0";

        fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
        if (fd < 0)
        {
            LOGF_ERROR("Failed to open %s", port);
            return false;
        }

        if (!configureSerial())
        {
            LOG_ERROR("Serial config failed (115200 8N1).");
            Disconnect();
            return false;
        }

        // Nano often auto-resets on port open; let it reboot.
        usleep(1500000);

        flushInput();

        std::string id;
        if (!cmdGetLine("ID", id))
        {
            LOG_ERROR("No response to ID");
            Disconnect();
            return false;
        }
        LOGF_INFO("Wheel ID: %s", id.c_str());

        // Optional: HOME on connect (recommended)
        std::string resp;
        cmdGetLine("HOME", resp);

        // Sync current position
        QueryFilter();

        return true;
    }

    bool Disconnect() override
    {
        if (fd >= 0)
        {
            close(fd);
            fd = -1;
        }
        return true;
    }

protected:
    bool SelectFilter(int position) override
    {
        if (position < 1 || position > NUM_SLOTS)
        {
            LOGF_ERROR("Requested filter %d out of range 1..%d", position, NUM_SLOTS);
            return false;
        }

        std::string resp;
        std::string cmd = "GOTO " + std::to_string(position);

        if (!cmdGetLine(cmd, resp))
        {
            LOGF_ERROR("No response to %s", cmd.c_str());
            return false;
        }

        if (resp.rfind("OK", 0) != 0)
        {
            LOGF_ERROR("Wheel error: %s", resp.c_str());
            return false;
        }

        CurrentFilter = position;
        return true;
    }

    // IMPORTANT: per your libindi headers, QueryFilter() returns int position or -1 on error.
    int QueryFilter() override
    {
        std::string resp;
        if (!cmdGetLine("POS?", resp))
            return -1;

        int pos = extractFirstInt(resp);
        if (pos >= 1 && pos <= NUM_SLOTS)
        {
            CurrentFilter = pos;
            return pos;
        }

        LOGF_ERROR("Bad POS? response: %s", resp.c_str());
        return -1;
    }

private:
    int fd = -1;

    bool configureSerial()
    {
        termios tty{};
        if (tcgetattr(fd, &tty) != 0)
            return false;

        cfmakeraw(&tty);
        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);

        tty.c_cflag |= (CLOCAL | CREAD);
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;

        tty.c_cc[VMIN]  = 0;
        tty.c_cc[VTIME] = 0;

        return (tcsetattr(fd, TCSANOW, &tty) == 0);
    }

    int readWithTimeout(char *buf, int maxlen, int timeoutMs)
    {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(fd, &set);

        timeval tv{};
        tv.tv_sec  = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;

        int rv = select(fd + 1, &set, nullptr, nullptr, &tv);
        if (rv <= 0)
            return 0;

        return (int)read(fd, buf, maxlen);
    }

    void flushInput()
    {
        char buf[256];
        while (readWithTimeout(buf, sizeof(buf), 50) > 0) {}
    }

    bool cmdGetLine(const std::string &cmd, std::string &out)
    {
        if (fd < 0)
            return false;

        std::string wire = cmd + "\n";
        if (write(fd, wire.c_str(), wire.size()) != (ssize_t)wire.size())
            return false;

        out.clear();
        char c = 0;
        for (int i = 0; i < 512; i++)
        {
            int n = readWithTimeout(&c, 1, 300);
            if (n <= 0)
                break;

            if (c == '\r')
                continue;

            if (c == '\n')
                return !out.empty();

            out.push_back(c);
        }

        return !out.empty();
    }

    static int extractFirstInt(const std::string &s)
    {
        for (size_t i = 0; i < s.size(); i++)
        {
            if (std::isdigit((unsigned char)s[i]))
                return std::atoi(s.c_str() + i);
        }
        return -1;
    }
};

static std::unique_ptr<DIYSerialFW> device;

extern "C" {
void ISGetProperties(const char *dev) { if (!device) device.reset(new DIYSerialFW()); device->ISGetProperties(dev); }
void ISInit() {}
void ISNewNumber(const char *d, const char *n, double v[], char *s[], int c) { device->ISNewNumber(d, n, v, s, c); }
void ISNewSwitch(const char *d, const char *n, ISState *v, char *s[], int c) { device->ISNewSwitch(d, n, v, s, c); }
void ISNewText(const char *d, const char *n, char *v[], char *s[], int c) { device->ISNewText(d, n, v, s, c); }
void ISNewBLOB(const char *, const char *, int[], int[], char **, char **, char **, int) {}
void ISSnoopDevice(XMLEle *root) { device->ISSnoopDevice(root); }
}
