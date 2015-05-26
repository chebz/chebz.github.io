<?php
require_once('../PHPMailer/class.phpmailer.php');
include("../PHPMailer/class.smtp.php"); // optional, gets called from within class.phpmailer.php if not already loaded


$mail             = new PHPMailer();

$body             = "test";

$mail->IsSMTP(); // telling the class to use SMTP
$mail->Host       = "mail.yourdomain.com"; // SMTP server
$mail->SMTPDebug  = 2;                     // enables SMTP debug information (for testing)
                                           // 1 = errors and messages
                                           // 2 = messages only
$mail->SMTPAuth   = true;                  // enable SMTP authentication
$mail->SMTPSecure = "tls";                 // sets the prefix to the servier
$mail->Host       = "smtp.gmail.com";      // sets GMAIL as the SMTP server
$mail->Port       = 587;                   // set the SMTP port for the GMAIL server
$mail->Username   = "chillpillgames@gmail.com";  // GMAIL username
$mail->Password   = "highlands77";            // GMAIL password

$mail->SetFrom('mikhail_naumov@yahoo.com', 'Mikhail Naumov');

$mail->Subject    = "PHPMailer Test Subject via smtp (Gmail), basic";

$mail->AltBody    = "To view the message, please use an HTML compatible email viewer!"; // optional, comment out and test

$mail->MsgHTML($body);

$address = "mikhail_naumov@yahoo.com";
$mail->AddAddress($address, "Mikhail Naumov");


if(!$mail->Send()) {
  echo "Mailer Error: " . $mail->ErrorInfo;
} else {
  echo "Message sent!";
}

return true;	
?>