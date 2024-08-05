# PowerShell >= 7.4
# [a-z0-9]

-join (1..100 | ForEach {(('0'..'9') + ('A'..'Z') | Get-SecureRandom)})
