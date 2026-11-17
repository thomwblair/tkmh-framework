#!/usr/bin/env python3
"""
TKMH .chiral Binary Format Validator and Parser
Conforming to the TKMH Normative Specification v1.0.0-rc.1
"""

import struct
import sys
import os

MAGIC_SIGNATURE = b'CHRL'
EXPECTED_VERSION = 0x00010000

def validate_chiral_file(filepath):
    if not os.path.exists(filepath):
        print(f"[ERROR] File not found: {filepath}")
        return False

    file_size = os.path.getsize(filepath)
    print(f"[INFO] Inspecting {filepath} ({file_size} bytes)...")

    with open(filepath, 'rb') as f:
        # 3.2 Header Byte Structure (44 bytes total)
        magic = f.read(4)
        if magic != MAGIC_SIGNATURE:
            print(f"[ERROR] Invalid magic signature: {magic!r}. Must be 'CHRL' (0x4348524C).")
            return False
        
        header_data = f.read(40)
        if len(header_data) < 40:
            print("[ERROR] File truncated: header is less than 44 bytes.")
            return False

        version, total_joints, flags, tensor_offset, wave_offset, weight_offset, kappa_limit = struct.unpack('<IIIQQQf', header_data)

        print(f"  -> Magic Signature: {magic.decode('ascii', errors='ignore')}")
        print(f"  -> Version: 0x{version:08X}")
        print(f"  -> Total Joints (N): {total_joints}")
        print(f"  -> Flags: 0x{flags:08X}")
        print(f"  -> Tensor Data Offset: 0x{tensor_offset:X}")
        print(f"  -> Wave Data Offset: 0x{wave_offset:X}")
        print(f"  -> Weight Data Offset: 0x{weight_offset:X}")
        print(f"  -> Global Kappa Limit: {kappa_limit:.4f}")

        if version != EXPECTED_VERSION:
            print(f"[WARNING] Version mismatch. Expected 0x{EXPECTED_VERSION:08X}, got 0x{version:08X}")
        
        if kappa_limit > 0.5:
            print(f"[WARNING] Unusually high kappa limit ({kappa_limit}). Standard normative limit is <= 0.20.")

        offsets = [('Tensor', tensor_offset), ('Wave', wave_offset), ('Weight', weight_offset)]
        for name, offset in offsets:
            if offset >= file_size:
                print(f"[ERROR] {name} Data Offset (0x{offset:X}) exceeds file size ({file_size} bytes).")
                return False

    print("[SUCCESS] .chiral binary header validation passed successfully.")
    return True

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python validate_chiral.py <file.chiral>")
        sys.exit(1)
    
    success = validate_chiral_file(sys.argv[1])
    sys.exit(0 if success else 1)