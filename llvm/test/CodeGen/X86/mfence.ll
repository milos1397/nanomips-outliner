; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i386-unknown-unknown -mattr=+sse2 | FileCheck %s --check-prefix=X32
; RUN: llc < %s -mtriple=x86_64-unknown-unknown -mattr=-sse2 | FileCheck %s --check-prefix=X64

; It doesn't matter if an x86-64 target has specified "no-sse2"; we still can use mfence.

define void @test() {
; X32-LABEL: test:
; X32:       # %bb.0:
; X32-NEXT:    mfence
; X32-NEXT:    retl
;
; X64-LABEL: test:
; X64:       # %bb.0:
; X64-NEXT:    mfence
; X64-NEXT:    retq
  fence seq_cst
  ret void
}

define i32 @fence(i32* %ptr) {
; X32-LABEL: fence:
; X32:       # %bb.0:
; X32-NEXT:    movl {{[0-9]+}}(%esp), %eax
; X32-NEXT:    mfence
; X32-NEXT:    movl (%eax), %eax
; X32-NEXT:    retl
;
; X64-LABEL: fence:
; X64:       # %bb.0:
; X64-NEXT:    mfence
; X64-NEXT:    movl (%rdi), %eax
; X64-NEXT:    retq
  %atomic = atomicrmw add i32* %ptr, i32 0 seq_cst
  ret i32 %atomic
}
