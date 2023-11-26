; ModuleID = 'hw2correct2.fplicm.bc'
source_filename = "hw2correct2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Test_1 = type { i64, i32 }
%struct.Test_0 = type { i32, i8, [100 x i32] }
%struct.Test = type { i32, i8, [100 x i32], i64, i32 }
%struct.Test1 = type { i32, i32 }

$__llvm_profile_raw_version = comdat any

$__profc_main = comdat nodeduplicate

@.str = private unnamed_addr constant [9 x i8] c"%d %d %d\00", align 1
@__llvm_profile_raw_version = hidden constant i64 72057594037927944, comdat
@__profc_main = private global [2 x i64] zeroinitializer, section "__llvm_prf_cnts", comdat, align 8
@__profd_main = private global { i64, i64, i64, ptr, ptr, i32, [2 x i16] } { i64 -2624081020897602054, i64 1063705162469825436, i64 sub (i64 ptrtoint (ptr @__profc_main to i64), i64 ptrtoint (ptr @__profd_main to i64)), ptr @main, ptr null, i32 2, [2 x i16] zeroinitializer }, section "__llvm_prf_data", comdat($__profc_main), align 8
@__llvm_prf_nm = private constant [14 x i8] c"\04\0Cx\DA\CBM\CC\CC\03\00\04\1B\01\A6", section "__llvm_prf_names", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @__profd_main], section "llvm.metadata"
@llvm.used = appending global [1 x ptr] [ptr @__llvm_prf_nm], section "llvm.metadata"
@myStructVar = internal global %struct.Test_1 zeroinitializer
@myStructVar.1 = internal global %struct.Test_0 zeroinitializer

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !prof !35 {
  %pgocount = load i64, ptr getelementptr inbounds ([2 x i64], ptr @__profc_main, i32 0, i32 1), align 8
  %1 = add i64 %pgocount, 1
  store i64 %1, ptr getelementptr inbounds ([2 x i64], ptr @__profc_main, i32 0, i32 1), align 8
  %2 = alloca i32, align 4
  %3 = alloca %struct.Test_1, align 8
  %4 = alloca %struct.Test_0, align 4
  %5 = alloca %struct.Test, align 8
  %6 = alloca %struct.Test1, align 4
  %7 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  store i32 0, ptr %7, align 4
  br label %8

8:                                                ; preds = %20, %0
  %9 = load i32, ptr %7, align 4
  %10 = icmp slt i32 %9, 10
  br i1 %10, label %11, label %24, !prof !36

11:                                               ; preds = %8
  %12 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 0
  store i32 10, ptr %12, align 8
  %13 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 0
  %14 = load i32, ptr %13, align 8
  %15 = add nsw i32 %14, 3
  %16 = trunc i32 %15 to i8
  %17 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 1
  store i8 %16, ptr %17, align 8
  %18 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 2
  %19 = getelementptr inbounds [100 x i32], ptr %18, i64 0, i64 3
  store i32 12, ptr %19, align 4
  br label %20

20:                                               ; preds = %11
  %pgocount1 = load i64, ptr @__profc_main, align 8
  %21 = add i64 %pgocount1, 1
  store i64 %21, ptr @__profc_main, align 8
  %22 = load i32, ptr %7, align 4
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr %7, align 4
  br label %8, !llvm.loop !37

24:                                               ; preds = %8
  %25 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 0
  %26 = load i32, ptr %25, align 8
  %27 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 1
  %28 = load i8, ptr %27, align 8
  %29 = sext i8 %28 to i32
  %30 = getelementptr inbounds %struct.Test, ptr %5, i32 0, i32 2
  %31 = getelementptr inbounds [100 x i32], ptr %30, i64 0, i64 3
  %32 = load i32, ptr %31, align 4
  %33 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %26, i32 noundef %29, i32 noundef %32)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: nounwind
declare void @llvm.instrprof.increment(ptr, i64, i32, i32) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!34}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 1, !"ProfileSummary", !6}
!6 = !{!7, !8, !9, !10, !11, !12, !13, !14, !15, !16}
!7 = !{!"ProfileFormat", !"InstrProf"}
!8 = !{!"TotalCount", i64 11}
!9 = !{!"MaxCount", i64 10}
!10 = !{!"MaxInternalCount", i64 1}
!11 = !{!"MaxFunctionCount", i64 10}
!12 = !{!"NumCounts", i64 2}
!13 = !{!"NumFunctions", i64 1}
!14 = !{!"IsPartialProfile", i64 0}
!15 = !{!"PartialProfileRatio", double 0.000000e+00}
!16 = !{!"DetailedSummary", !17}
!17 = !{!18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33}
!18 = !{i32 10000, i64 0, i32 0}
!19 = !{i32 100000, i64 10, i32 1}
!20 = !{i32 200000, i64 10, i32 1}
!21 = !{i32 300000, i64 10, i32 1}
!22 = !{i32 400000, i64 10, i32 1}
!23 = !{i32 500000, i64 10, i32 1}
!24 = !{i32 600000, i64 10, i32 1}
!25 = !{i32 700000, i64 10, i32 1}
!26 = !{i32 800000, i64 10, i32 1}
!27 = !{i32 900000, i64 10, i32 1}
!28 = !{i32 950000, i64 10, i32 1}
!29 = !{i32 990000, i64 10, i32 1}
!30 = !{i32 999000, i64 10, i32 1}
!31 = !{i32 999900, i64 10, i32 1}
!32 = !{i32 999990, i64 10, i32 1}
!33 = !{i32 999999, i64 10, i32 1}
!34 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!35 = !{!"function_entry_count", i64 1}
!36 = !{!"branch_weights", i32 10, i32 1}
!37 = distinct !{!37, !38}
!38 = !{!"llvm.loop.mustprogress"}
