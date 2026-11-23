  switch(e.at(0x58)) {
  case 1:
    e.at(0x50) = 0xfffffffe;
    e.at(0x68) = 0;
    iVar4 = e.at(0x70) + 1;
    e.at(0x70) = iVar4;
    if (0x32 < iVar4) {
      e.at(0x70) = 0;
      pose(4);
    }
    if (100 < e.at(0x6c)) {
      pose(6);
      e.at(0x6c) = 0;
    }
    break;
  case 2:
    e.at(0x50) = 2;
    e.at(0x68) = 1;
    iVar4 = e.at(0x74) + 1;
    e.at(0x74) = iVar4;
    if (0x32 < iVar4) {
      e.at(0x74) = 0;
      pose(5);
    }
    if (100 < e.at(0x6c)) {
      pose(6);
      e.at(0x6c) = 0;
    }
    break;
  case 3:
    if ((e.at(0x18) == 0) && (e.at(0x1c) == 0)) {
      e.at(0x54) = 0xfffffff6;
    }
    if (e.at(0x18) == 1) {
      if (e.at(0x1c) == 0) {
        e.at(0x24) = 10;
        e.at(0x54) = 10;
        e.state=2;defeated=true;
        
      }
      if ((e.at(0x18) == 1) && (e.at(0x1c) == 9)) {
        e.state=2;defeated=true;
      }
    }
    break;
  case 4:
    e.at(0x50) = 0xfffffffc;
    e.at(0x68) = 0;
    if (100 < e.at(0x6c)) {
      pose(6);
      e.at(0x6c) = 0;
    }
    break;
  case 5:
    e.at(0x50) = 4;
    e.at(0x68) = 1;
    if (100 < e.at(0x6c)) {
      pose(6);
      e.at(0x6c) = 0;
    }
    break;
  case 6:
    e.at(0x50) = 0xfffffffc;
    if (e.at(0x68) != 0) {
      e.at(0x50) = 4;
    }
    if ((e.at(0x18) == 2) && (e.at(0x1c) == 9)) {
      if (p.at(8) < e.at(4)) {
        pose(1);
      }
      else {
        pose(2);
      }
    }
  }
