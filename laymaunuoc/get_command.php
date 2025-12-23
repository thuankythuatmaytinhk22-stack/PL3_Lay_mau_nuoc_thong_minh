<?php
$command_file = "command.txt";

// --- PHẦN MỚI: Xử lý khi ESP32 báo nước đầy ---
if (isset($_GET['status']) && $_GET['status'] == 'full') {
    if (file_exists($command_file)) {
        $current_content = file_get_contents($command_file);
        // Chuyển tất cả ON thành OFF, giữ nguyên ngưỡng MAX
        $new_content = str_replace(['R1_ON', 'R2_ON', 'BOTH_ON'], ['R1_OFF', 'R2_OFF', 'BOTH_OFF'], $current_content);
        file_put_contents($command_file, $new_content);
    }
}
// ----------------------------------------------

if (!file_exists($command_file)) {
    echo "R1_OFF,R2_OFF,MAX:8";
    exit();
}

$cmd = trim(file_get_contents($command_file));
if (empty($cmd)) {
    echo "R1_OFF,R2_OFF,MAX:8";
    exit();
}

$output = str_replace('|', ',', $cmd);
$output = str_replace('BOTH_ON', 'R1_ON,R2_ON', $output);
$output = str_replace('BOTH_OFF', 'R1_OFF,R2_OFF', $output);

echo $output;
?>