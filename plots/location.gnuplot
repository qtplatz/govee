
govee_addr = substr(ARG2,17,28)

if ( govee_addr eq 'A4C138E4544D' ) {
    location[ARG1] = '2F Hall'
} else {
    if ( govee_addr eq 'A4C138258E2F' ) {
	location[ARG1] = '1F Living'
    } else {
	location[ARG1] = 'Unknown'
    }
}
