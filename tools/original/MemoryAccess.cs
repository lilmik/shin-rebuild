using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
public static class ShinBossMemory {
 [DllImport("kernel32.dll", SetLastError=true)] public static extern IntPtr OpenProcess(uint access,bool inherit,int pid);
 [DllImport("kernel32.dll")] public static extern bool CloseHandle(IntPtr handle);
 [DllImport("kernel32.dll", SetLastError=true)] static extern bool ReadProcessMemory(IntPtr h,IntPtr address,byte[] data,UIntPtr size,out UIntPtr count);
 [DllImport("kernel32.dll", SetLastError=true)] static extern bool WriteProcessMemory(IntPtr h,IntPtr address,byte[] data,UIntPtr size,out UIntPtr count);
 public static byte[] Read(IntPtr h,long address,int size){byte[] b=new byte[size];UIntPtr n;if(!ReadProcessMemory(h,new IntPtr(address),b,new UIntPtr((uint)size),out n)||n.ToUInt64()!=(ulong)size)throw new Win32Exception();return b;}
 public static int I32(IntPtr h,long address){return BitConverter.ToInt32(Read(h,address,4),0);}
 public static void Write(IntPtr h,long address,byte[] b){UIntPtr n;if(!WriteProcessMemory(h,new IntPtr(address),b,new UIntPtr((uint)b.Length),out n)||n.ToUInt64()!=(ulong)b.Length)throw new Win32Exception();}
 public static void Put(byte[] b,int offset,int value){Array.Copy(BitConverter.GetBytes(value),0,b,offset,4);}
}
