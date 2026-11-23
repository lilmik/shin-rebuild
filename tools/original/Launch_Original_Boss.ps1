# Developer comparison tool: only the launched original process is modified.
# Requires the exact executable SHA-256 below; never edits executable bytes on disk.
# Stage 7 must first be entered manually; addresses do not apply to other builds.
param([Parameter(Mandatory=$true)][string]$OriginalGame)
$ErrorActionPreference='Stop'
Add-Type -Path (Join-Path $PSScriptRoot 'MemoryAccess.cs')
$gameRoot=(Resolve-Path -LiteralPath $OriginalGame).Path
$exe=Join-Path $gameRoot 'SHIN.exe'
if((Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash -ne 'c32b511751251a8d55d3788dc3b19c0dd614f0056f80f4b4bb66fc2b0f9b161d'){throw 'Original executable differs from the analyzed build.'}
Write-Host 'Choose stage 7 in the ORIGINAL game. After it starts, this launcher moves you to the Boss trigger once.'
$gameProcess=Start-Process -FilePath $exe -WorkingDirectory $gameRoot -PassThru
$handle=[ShinBossMemory]::OpenProcess(0x438,$false,$gameProcess.Id)
if($handle -eq [IntPtr]::Zero){throw 'Could not open the launched game.'}
try {
 $deadline=(Get-Date).AddMinutes(5)
 while((Get-Date) -lt $deadline){
  $gameProcess.Refresh();if($gameProcess.HasExited){throw 'Original game exited before stage 7 was ready.'}
  try {
   $imageBase=$gameProcess.MainModule.BaseAddress.ToInt64();$delta=$imageBase-0x400000
   $stage=[ShinBossMemory]::I32($handle,0x461428+$delta)
   $player=[long][ShinBossMemory]::I32($handle,0x473d20+$delta)
   $camera=[long][ShinBossMemory]::I32($handle,0x77bf48+$delta)
   $ready=$stage -eq 6 -and $player -gt 65536 -and $camera -gt 65536
   if($ready){$ready=[ShinBossMemory]::I32($handle,$player+0x100) -eq 1}
  } catch {$ready=$false}
  if($ready){
   $px=18114;$py=3072-[ShinBossMemory]::I32($handle,$player+0x38)+16
   $cx=[Math]::Max(0,$px-280);$cy=[Math]::Min(2720,[Math]::Max(0,$py-260))
   $xy=New-Object byte[] 8;[ShinBossMemory]::Put($xy,0,$px);[ShinBossMemory]::Put($xy,4,$py)
   [ShinBossMemory]::Write($handle,$player+8,$xy);[ShinBossMemory]::Write($handle,$player+0x4c,$xy)
   $velocity=New-Object byte[] 8;[ShinBossMemory]::Write($handle,$player+0x54,$velocity)
   $cam=[ShinBossMemory]::Read($handle,$camera,0x44)
   [ShinBossMemory]::Put($cam,4,$cx);[ShinBossMemory]::Put($cam,8,$cy)
   [ShinBossMemory]::Put($cam,0xc,[int][Math]::Floor($cx/32));[ShinBossMemory]::Put($cam,0x10,[int][Math]::Floor($cy/32))
   [ShinBossMemory]::Put($cam,0x14,($cx%32));[ShinBossMemory]::Put($cam,0x18,($cy%32))
   [ShinBossMemory]::Put($cam,0x1c,$cx);[ShinBossMemory]::Put($cam,0x20,$cy)
   [ShinBossMemory]::Put($cam,0x24,0);[ShinBossMemory]::Put($cam,0x28,0);[ShinBossMemory]::Put($cam,0x2c,0)
   [ShinBossMemory]::Write($handle,$camera,$cam)
   Write-Host 'Moved to the original stage-7 Boss trigger. Continue in the game window.';exit 0
  }
  Start-Sleep -Milliseconds 50
 }
 throw 'Timed out waiting for stage 7. Run the launcher again and choose stage 7.'
} finally {[ShinBossMemory]::CloseHandle($handle)|Out-Null}
