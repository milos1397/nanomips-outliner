// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py
// RUN: %clang_cc1 %s -emit-llvm -ffp-exception-behavior=maytrap -o - -triple x86_64-unknown-unknown | FileCheck %s

// Test that the constrained intrinsics are picking up the exception
// metadata from the AST instead of the global default from the command line.
// FIXME: these functions shouldn't trap on SNaN.

#pragma float_control(except, on)

int printf(const char *, ...);

// CHECK-LABEL: @p(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[STR_ADDR:%.*]] = alloca i8*, align 8
// CHECK-NEXT:    [[X_ADDR:%.*]] = alloca i32, align 4
// CHECK-NEXT:    store i8* [[STR:%.*]], i8** [[STR_ADDR]], align 8
// CHECK-NEXT:    store i32 [[X:%.*]], i32* [[X_ADDR]], align 4
// CHECK-NEXT:    [[TMP0:%.*]] = load i8*, i8** [[STR_ADDR]], align 8
// CHECK-NEXT:    [[TMP1:%.*]] = load i32, i32* [[X_ADDR]], align 4
// CHECK-NEXT:    [[CALL:%.*]] = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str, i64 0, i64 0), i8* [[TMP0]], i32 [[TMP1]]) [[ATTR4:#.*]]
// CHECK-NEXT:    ret void
//
void p(char *str, int x) {
  printf("%s: %d\n", str, x);
}

#define P(n,args) p(#n #args, __builtin_##n args)

// CHECK-LABEL: @test_long_double_isinf(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[LD_ADDR:%.*]] = alloca x86_fp80, align 16
// CHECK-NEXT:    store x86_fp80 [[D:%.*]], x86_fp80* [[LD_ADDR]], align 16
// CHECK-NEXT:    [[TMP0:%.*]] = load x86_fp80, x86_fp80* [[LD_ADDR]], align 16
// CHECK-NEXT:    [[BITCAST:%.*]] = bitcast x86_fp80 [[TMP0]] to i80
// CHECK-NEXT:    [[SHL1:%.*]] = shl i80 [[BITCAST]], 1
// CHECK-NEXT:    [[CMP:%.*]] = icmp eq i80 [[SHL1]], -18446744073709551616
// CHECK-NEXT:    [[RES:%.*]] = zext i1 [[CMP]] to i32
// CHECK-NEXT:    call void @p(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i64 0, i64 0), i32 [[RES]]) [[ATTR4]]
// CHECK-NEXT:    ret void
//
void test_long_double_isinf(long double ld) {
  P(isinf, (ld));

  return;
}

// CHECK-LABEL: @test_long_double_isnan(
// CHECK-NEXT:  entry:
// CHECK-NEXT:    [[LD_ADDR:%.*]] = alloca x86_fp80, align 16
// CHECK-NEXT:    store x86_fp80 [[D:%.*]], x86_fp80* [[LD_ADDR]], align 16
// CHECK-NEXT:    [[TMP0:%.*]] = load x86_fp80, x86_fp80* [[LD_ADDR]], align 16
// CHECK-NEXT:    [[BITCAST:%.*]] = bitcast x86_fp80 [[TMP0]] to i80
// CHECK-NEXT:    [[ABS:%.*]] = and i80 [[BITCAST]], 604462909807314587353087
// CHECK-NEXT:    [[TMP1:%.*]] = sub i80 604453686435277732577280, [[ABS]]
// CHECK-NEXT:    [[ISNAN:%.*]] = lshr i80 [[TMP1]], 79
// CHECK-NEXT:    [[RES:%.*]] = trunc i80 [[ISNAN]] to i32
// CHECK-NEXT:    call void @p(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i64 0, i64 0), i32 [[RES]]) [[ATTR4]]
// CHECK-NEXT:    ret void
//
void test_long_double_isnan(long double ld) {
  P(isnan, (ld));

  return;
}