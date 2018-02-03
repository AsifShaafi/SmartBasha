<?php

require_once "../include/config.php";

$query = "SELECT * FROM channel_state";

$channel = array();

$result = mysqli_query($conn, $query) or die(mysqli_error($conn));

if ($result) {

    // echo "\ninside result " . mysqli_affected_rows($connection) . "\n";

    while($ch = mysqli_fetch_assoc($result)) {
        // echo "\nchannel name: " . $ch['channel_name'] . "\n";
        $channel[$ch['channel_name']] = $ch['channel_state'] == 0 ? "off" : "on";
    }

}
else {
    echo "failed";
}

echo json_encode(
        $channel
    );