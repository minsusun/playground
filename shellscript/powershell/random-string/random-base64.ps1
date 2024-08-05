# PowerShell >= 7.4
# base64

[Convert]::ToBase64String(((1..100) | % { "{0:d}" -f [byte]( Get-SecureRandom -min 0 -max 128 )}))
