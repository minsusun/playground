# PowerShell >= 7.4
# base64

Get-SecureRandom -Maximum 255 -Count 128 | Join-String -FormatString "{0:X}"
