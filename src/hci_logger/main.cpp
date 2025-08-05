// hci_raw_logger.cpp
#include "../ble_scanner/bt_address.hpp"
#include "parse_govee_payload.hpp"
#include <adportable/debug.hpp>
#include <boost/endian/conversion.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <sstream>

namespace ble {
    namespace legacy {
        constexpr size_t Event_Type_OFFSET   = 0; // 1 o  0
        constexpr size_t Address_Type_OFFSET = 1; // 1 o  1
        constexpr size_t Address_OFFSET      = 2; // 6 o  2..7
        constexpr size_t Data_Length_OFFSET  = 8; // 1 o
        constexpr size_t AD_Data_OFFSET      = 9; // + data length
        // p+9+L : RSSI               (1)

        class data_frame {
            std::basic_string_view< uint8_t > data_;
        public:
            data_frame( std::basic_string_view< uint8_t > sv ) : data_( sv ) {}
            uint8_t event_type() const { return data_.at( Event_Type_OFFSET ); }
            uint8_t address_type() const { return data_.at( Address_Type_OFFSET ); }
            bt_address address() const {
                return bt_address{ *reinterpret_cast<const bdaddr_t*>(data_.data() + Address_OFFSET) };
            }
            uint8_t data_length() const { return data_.at( Data_Length_OFFSET );  }
            std::basic_string_view< const uint8_t > ad_data() const {
                return std::basic_string_view< const uint8_t >{ (data_.data() + AD_Data_OFFSET), data_length() };
            }
            uint8_t rssi() const { return data_.at( AD_Data_OFFSET + data_length() ); }
            std::string to_string() const {
                std::ostringstream o;
                o << std::format( "event_type: {:02x}", event_type() )
                  << std::format( ", address_type: {:02x}", address_type() )
                  << std::format( ", address: {}", address().to_string() )
                  << std::format( ", data_length: {}", data_length() );
                o << std::endl;
                auto ad = ad_data();
                o << "AD Data:\t";
                for ( auto it = ad.begin(); it != ad.end(); ++it )
                    o << std::format( "{:02x} ", *it );
                return o.str();
            }

        };
    }

    namespace extended {
        /* p+0..p+1  : */ constexpr size_t Event_Type_OFFSET      = 0; //  2 o  (2)
        /* p+2       : */ constexpr size_t Address_Type_OFFSET    = 2; //       (1)
        /* p+3..p+8  : */ constexpr size_t Address_OFFSET         = 3; //;      (6)
        /* p+9       : */ constexpr size_t Primary_PHY_OFFSET     = 9; //       (1)
        /* p+10      : */ constexpr size_t Secondary_PHY_OFFSET   = 10; //      (1)
        /* p+11      : */ constexpr size_t Advertising_SID_OFFSET = 11; //      (1) (0xFF if N/A)
        /* p+12      : */ constexpr size_t Tx_Power_OFFSET        = 12; //      (1) (0x7F if N/A)
        /* p+13      : */ constexpr size_t RSSI_OFFSET            = 13; //      (1) (signed)
        /* p+14..15  : */ constexpr size_t Periodic_Adv_Interval_OFFSET = 14; //(2, LE)
        /* p+16      : */ constexpr size_t Direct_Address_Type_OFFSET = 16; //  (1)
        /* p+17..22  : */ constexpr size_t Direct_Address_OFFSET  = 17; //      (6)
        /* p+23      : */ constexpr size_t Data_Length_OFFSET     = 23; // L    (1)
        /* p+24..    : */ constexpr size_t AD_Data_OFFSET         = 24; //(L bytes)        <-- walk TLVs here

        class data_frame {
            std::basic_string_view< uint8_t > data_;
        public:
            data_frame( std::basic_string_view< uint8_t > sv ) : data_( sv ) {}
            uint16_t event_type() const {
                return boost::endian::big_to_native( *reinterpret_cast< const uint16_t * >( data_.data() ));
            }
            uint8_t address_type() const { return data_.at( Address_Type_OFFSET ); } // 2
            bt_address address() const {
                return bt_address{ *reinterpret_cast<const bdaddr_t*>(data_.data() + Address_OFFSET) };
            }
            uint8_t primary_phy() const { return data_.at( Primary_PHY_OFFSET ); }
            uint8_t secondary_phy() const { return data_.at( Secondary_PHY_OFFSET ); }
            uint8_t advertising_sid() const { return data_.at( Advertising_SID_OFFSET ); }
            uint8_t tx_power() const { return data_.at( Tx_Power_OFFSET ); }
            uint8_t rssi() const { return data_.at( RSSI_OFFSET ); }
            uint16_t periodic_adv_interval() const {
                return boost::endian::big_to_native( uint16_t( data_.at( Direct_Address_OFFSET ) ) << 8
                                                     | data_.at( Direct_Address_OFFSET + 1 ) );
            }
            uint8_t direct_address_type() const { return data_.at( Direct_Address_OFFSET ); };
            uint16_t direct_address() const {
                return boost::endian::big_to_native( uint16_t( data_.at( 17 ) ) << 16 | data_.at( 18 ) );
            }
            uint8_t data_length() const { return data_.at( Data_Length_OFFSET ); };
            std::basic_string_view< const uint8_t > ad_data() const {
                return std::basic_string_view< const uint8_t >{ (data_.data() + AD_Data_OFFSET), data_length() };
            }
            std::string to_string() const {
                std::ostringstream o;
                o << std::format( "event_type: {:04x}", event_type() )
                  << std::format( ", address_type: {:02x}", address_type() )
                  << std::format( ", address: {}", address().to_string() )
                  << std::format( ", primary_phy: {:02x}", primary_phy() )
                  << std::format( ", secondary_phy: {:02x}", secondary_phy() )
                  << std::format( ", ad_sid: {:02x}", advertising_sid() )
                  << std::format( ", tx_power: {:02x}", tx_power() )
                  << std::format( ", rssi: {:02x}", rssi() )
                  << std::format( ", interval: {:04x}", periodic_adv_interval() )
                  << std::format( ", direct_addr_type: {:02x}", direct_address_type() )
                  << std::format( ", direct_addr: {:04x}", direct_address() )
                  << std::format( ", data_length: {}", data_length() );
                o << std::endl;
                auto ad = ad_data();
                for ( auto it = ad.begin(); it != ad.end(); ++it )
                    o << std::format( "{:02x} ", *it );
                return o.str();
            }
        };

    }
}

int
main()
{
    int dev_id = hci_get_route(nullptr);
    if (dev_id < 0) {
        std::cerr << "No Bluetooth adapter found" << std::endl;
        return 1;
    }

    int sock = socket(AF_BLUETOOTH, SOCK_RAW | SOCK_NONBLOCK, BTPROTO_HCI);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_hci addr{};
    addr.hci_family = AF_BLUETOOTH;
    addr.hci_dev = dev_id;
    addr.hci_channel = HCI_CHANNEL_RAW;

    if ( bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0 ) {
        perror("bind");
        close(sock);
        return 1;
    }

    // Set filter to only LE Meta Events (0x3e)
    struct hci_filter flt;
    hci_filter_clear(&flt);
    hci_filter_set_ptype(HCI_EVENT_PKT, &flt);
    hci_filter_set_event(EVT_LE_META_EVENT, &flt);

    if (setsockopt(sock, SOL_HCI, HCI_FILTER, &flt, sizeof(flt)) < 0) {
        perror("setsockopt");
        close(sock);
        return 1;
    }

    std::cout << "Listening for LE advertising packets on hci" << dev_id << "..." << std::endl;

    const bt_address target{ "a4:c1:38:e4:54:4d" }; // | A4:C1:38:25:8E:2F
    const bt_address target2{ "a4:c1:38:25:8e:2f" };
    std::vector< bt_address > targets( {bt_address( "a4:c1:38:e4:54:4d" ), bt_address( "a4:c1:38:25:8e:2f" )} );
    const uint8_t __pat[] = {0xff, 0x88, 0xec};
    const std::basic_string_view< uint8_t > pat{ __pat, 3 };

    for ( const auto& add: targets )
        ADDEBUG() << "looking for: " << add;

    while ( true ) {

        std::array< uint8_t, HCI_MAX_EVENT_SIZE > buf = {};
        // uint8_t buf[ HCI_MAX_EVENT_SIZE ] = {};

        int len = read( sock, buf.data(), HCI_MAX_EVENT_SIZE );

        if ( len > 14 ) {
            bt_address addr( *reinterpret_cast<const bdaddr_t*>(buf.data() + 7) );
            if ( addr == targets[0] || addr == targets[1] ) {
                ADDEBUG() << std::format("{} octets data received from: ", len) << addr;
                // std::basic_string_view< uint8_t > sv{ buf.begin() + 14, size_t(len - 14) };
                std::basic_string_view< uint8_t > sv{ buf.data(), size_t(len) };
                ADDEBUG() << std::make_tuple( "HCI_EVENT_PKT: ", int(sv.at(0)), std::format("\t{:x} == 0x04", sv.at(0) ));
                ADDEBUG() << std::make_tuple( "EVENT_CODE:    ", int(sv.at(1)), std::format("\t{:x} == 0x3e", sv.at(1) ));
                ADDEBUG() << std::make_tuple( "PAYLOAD LEN:   ", int(sv.at(2))  );
                ADDEBUG() << std::make_tuple( "SUBEVENT       ", int(sv.at(3)), (sv.at(3) == 0x02 ? "\tlegacy" : "\textended") );
                ADDEBUG() << std::make_tuple( "Num_Reports    ", int(sv.at(4)) );
                ADDEBUG() << std::format("read buffer size {} == {} (payload_len + 3)", len, sv.at(2) + 3 );

                if ( sv.at(3) == 0x02 ) { // legacy
                    using namespace ble::legacy;
                    data_frame df( std::basic_string_view< uint8_t >( sv.begin() + 5, sv.size() - 5 ) );
                    ADDEBUG() << df.to_string();

                } else if ( sv.at(3) == 0x0d ) { // extended
                    using namespace ble::extended;
                    data_frame df( std::basic_string_view< uint8_t >( sv.begin() + 5, sv.size() - 5 ) );
                    ADDEBUG() << df.to_string();
                }

                for ( auto it = sv.begin(); it != sv.end(); ++it ) {
                    std::cout << std::format( "{:02x} ", *it );
                    if ( (std::distance( sv.begin(), it ) + 1) % 16 == 0 )
                        std::cout << std::endl;
                }
                std::cout << std::endl;
                auto pos = sv.find( pat );
                // if ( pos != std::basic_string_view< uint8_t >::npos )
                //     parse_govee_payload( sv.begin() + pos, sv.size() - pos );
            }
        } else {
            usleep(10000); // 10ms delay to avoid tight polling
        }
    }

    close(sock);
    return 0;
}
