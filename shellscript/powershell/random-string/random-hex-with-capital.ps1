# PowerShell >= 7.4
# [A-Za-z0-9]

-join (1..100 | ForEach {(('0'..'9') + ('A'..'Z') + ('a'..'z') | Get-SecureRandom)})
