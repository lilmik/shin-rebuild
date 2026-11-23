
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x45580  517  callback_00445580 */

  if (param_2 == 1) {
    iVar3 = 0;
    iVar4 = 0;
    do {
      iVar2 = probe(iVar4,e.at(4),e.at(8) - iVar3);
      if (iVar2 == 0) {
        e.at(8) = e.at(8) - iVar3;
        e.at(0x40) = -e.at(0x40);
        iVar4 = e.at(0x44) + 1;
        e.at(0x44) = iVar4;
        if (iVar4 != 1) {
          return;
        }
        e.state = 2;
        return;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x1a);
    iVar3 = e.at(4);
    if (e.at(0x34) < iVar3) {
      do {
        iVar2 = probe(iVar4,iVar3,e.at(0x38));
        if (iVar2 == 0) {
          e.at(4) = iVar3;
          e.at(0x3c) = -e.at(0x3c);
          iVar3 = e.at(0x44) + 1;
          e.at(0x44) = iVar3;
          goto joined_r0x00445656;
        }
        iVar3 = iVar3 + -1;
      } while (e.at(0x34) <= iVar3);
    }
    else if (iVar3 < e.at(0x34)) {
LAB_00445627:
      iVar2 = probe(iVar4,iVar3,e.at(0x38));
      if (iVar2 != 0) goto code_r0x00445639;
      e.at(4) = iVar3;
      e.at(0x3c) = -e.at(0x3c);
      iVar3 = e.at(0x44) + 1;
      e.at(0x44) = iVar3;
joined_r0x00445656:
      if (iVar3 == 1) {
        e.state = 2;
      }
    }
LAB_0044565c:
    iVar3 = e.at(0x38);
    iVar2 = e.at(8);
    if (iVar3 < iVar2) {
      if (iVar3 <= iVar2) {
        while (iVar3 = probe(iVar4,e.at(4),iVar2), iVar3 != 0) {
          iVar2 = iVar2 + -1;
          if (iVar2 < e.at(0x38)) {
            return;
          }
        }
        e.at(8) = iVar2;
        e.at(0x40) = -e.at(0x40);
        iVar4 = e.at(0x44) + 1;
        e.at(0x44) = iVar4;
        if (iVar4 == 1) {
          e.state = 2;
          return;
        }
      }
    }
    else if ((iVar2 < iVar3) && (iVar2 <= iVar3)) {
      while (iVar3 = probe(iVar4,e.at(4),iVar2), iVar3 != 0) {
        iVar2 = iVar2 + 1;
        if (e.at(0x38) < iVar2) {
          return;
        }
      }
      e.at(8) = iVar2;
      e.at(0x40) = -e.at(0x40);
      iVar4 = e.at(0x44) + 1;
      e.at(0x44) = iVar4;
      if (iVar4 == 1) {
        e.state = 2;
        return;
      }
    }
  }
  else {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(0,e.at(4),e.at(8) - iVar4);
      if (iVar3 == 0) {
        e.at(8) = e.at(8) - iVar4;
        iVar4 = e.at(0x40) + 2;
        e.at(0x40) = iVar4;
        e.at(0x40) = -iVar4;
        iVar4 = e.at(0x44) + 1;
        e.at(0x44) = iVar4;
        if (iVar4 != 1) {
          return;
        }
        e.state = 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = e.at(8);
    if ((e.at(0x38) < iVar4) && (e.at(0x38) <= iVar4)) {
      while (iVar3 = probe(0,e.at(4),iVar4), iVar3 != 0) {
        iVar4 = iVar4 + -1;
        if (iVar4 < e.at(0x38)) {
          return;
        }
      }
      e.at(8) = iVar4;
      e.at(0x40) = -e.at(0x40);
      iVar4 = e.at(0x44) + 1;
      e.at(0x44) = iVar4;
      if (iVar4 == 1) {
        e.state = 2;
      }
    }
  }
  return;
code_r0x00445639:
  iVar3 = iVar3 + 1;
  if (e.at(0x34) < iVar3) goto LAB_0044565c;
  goto LAB_00445627;
