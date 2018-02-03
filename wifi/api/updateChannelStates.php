<?php

require_once "../include/config.php";

if (isset($_POST['submit'])) {
    $channelID = $_POST['channel'];
    $state = $_POST['state'];

    $stateID;

    echo "\n" . $state . "\n";

    if ( strpos( $state, "on" ) != false ) {
        $stateID = 1;
    }
    else {
        $stateID = 0;
    }

    echo "\n" . $stateID . "\n";

    $query = "INSERT INTO channel_state VALUES ( null, '{$channelID}', {$stateID}) ON DUPLICATE KEY UPDATE channel_state = '{$stateID}'";

    $result = mysqli_query($conn, $query);

    if ($result) {
        echo "success: " . $state;
    }
    else {
        echo "failed";
    }
}