while($true)
{
    $text = (Get-Date -Format "yyMMdd-HHmmss")
    $msg = "feat(dump): " + $text
    $text | Set-Content $PSScriptRoot\change.txt
    $text | Add-Content $PSScriptRoot\append.txt
    git add .
    git commit -m $msg
    git push
    Start-Sleep -Seconds 30
}