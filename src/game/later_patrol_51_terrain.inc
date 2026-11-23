
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
                    /* 0x1e060  185  callback_0041e060 */

  if (param_2 != 1) {
    if (param_2 != 2) {
      return;
    }
    iVar4 = 0;
    do {
      iVar3 = probe(0,e.at(4),e.at(8) - iVar4);
      if (iVar3 == 0) {
        e.at(8) = e.at(8) - iVar4;
        e.at(0x54) = e.at(0x54) + 2;
        return;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x1a);
    iVar4 = e.at(8);
    if (iVar4 <= e.at(0x4c)) {
      return;
    }
    do {
      iVar3 = probe(0,e.at(4),iVar4);
      if (iVar3 == 0) {
        e.at(8) = iVar4;
        return;
      }
      iVar4 = iVar4 + -1;
    } while (e.at(0x4c) <= iVar4);
    return;
  }
  iVar3 = 0;
  iVar4 = 0;
  do {
    iVar2 = probe(iVar4,e.at(4),e.at(8) - iVar3);
    if (iVar2 == 0) {
      e.at(8) = e.at(8) - iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x1a);
  iVar3 = e.at(4);
  if (e.at(0x48) < iVar3) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x4c));
      if (iVar2 == 0) goto LAB_0041e0e9;
      iVar3 = iVar3 + -1;
    } while (e.at(0x48) <= iVar3);
  }
  else if (iVar3 < e.at(0x48)) {
    do {
      iVar2 = probe(iVar4,iVar3,e.at(0x4c));
      if (iVar2 == 0) goto LAB_0041e0e9;
      iVar3 = iVar3 + 1;
    } while (iVar3 <= e.at(0x48));
  }
LAB_0041e0ec:
  iVar3 = e.at(8);
  if (e.at(0x4c) < iVar3) {
    do {
      iVar2 = probe(iVar4,e.at(4),iVar3);
      if (iVar2 == 0) {
        e.at(8) = iVar3;
        return;
      }
      iVar3 = iVar3 + -1;
    } while (e.at(0x4c) <= iVar3);
    return;
  }
  if (e.at(0x4c) <= iVar3) {
    return;
  }
  do {
    iVar2 = probe(iVar4,e.at(4),iVar3);
    if (iVar2 == 0) {
      e.at(8) = iVar3;
      return;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 <= e.at(0x4c));
  return;
LAB_0041e0e9:
  e.at(4) = iVar3;
  goto LAB_0041e0ec;
