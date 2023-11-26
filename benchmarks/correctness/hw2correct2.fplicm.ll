; ModuleID = 'hw2correct2.fplicm.bc'
source_filename = "hw2correct2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%MyStruct = type { i32, i32, i32 }
%struct.Test = type { i32, i32, i64, i8 }

$__llvm_profile_raw_version = comdat any

$__profc_main = comdat nodeduplicate

@__llvm_profile_raw_version = hidden constant i64 72057594037927944, comdat
@__profc_main = private global [2 x i64] zeroinitializer, section "__llvm_prf_cnts", comdat, align 8
@__profd_main = private global { i64, i64, i64, ptr, ptr, i32, [2 x i16] } { i64 -2624081020897602054, i64 1063705162469825436, i64 sub (i64 ptrtoint (ptr @__profc_main to i64), i64 ptrtoint (ptr @__profd_main to i64)), ptr @main, ptr null, i32 2, [2 x i16] zeroinitializer }, section "__llvm_prf_data", comdat($__profc_main), align 8
@__llvm_prf_nm = private constant [14 x i8] c"\04\0Cx\DA\CBM\CC\CC\03\00\04\1B\01\A6", section "__llvm_prf_names", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @__profd_main], section "llvm.metadata"
@llvm.used = appending global [1 x ptr] [ptr @__llvm_prf_nm], section "llvm.metadata"
@myStructVar = internal global %MyStruct zeroinitializer

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !prof !35 {
  %pgocount = load i64, ptr getelementptr inbounds ([2 x i64], ptr @__profc_main, i32 0, i32 1), align 8
  %1 = add i64 %pgocount, 1
  store i64 %1, ptr getelementptr inbounds ([2 x i64], ptr @__profc_main, i32 0, i32 1), align 8
  %2 = alloca i32, align 4
  %3 = alloca [10 x %struct.Test], align 16
  %4 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  store i32 0, ptr %4, align 4
  br label %5

5:                                                ; preds = %24, %0
  %6 = load i32, ptr %4, align 4
  %7 = icmp slt i32 %6, 10
  br i1 %7, label %8, label %28, !prof !36

8:                                                ; preds = %5
  %9 = load i32, ptr %4, align 4
  %10 = sext i32 %9 to i64
  %11 = getelementptr inbounds [10 x %struct.Test], ptr %3, i64 0, i64 %10
  %12 = getelementptr inbounds %struct.Test, ptr %11, i32 0, i32 0
  store i32 10, ptr %12, align 8
  %13 = load i32, ptr %4, align 4
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds [10 x %struct.Test], ptr %3, i64 0, i64 %14
  %16 = getelementptr inbounds %struct.Test, ptr %15, i32 0, i32 0
  %17 = load i32, ptr %16, align 8
  %18 = add nsw i32 %17, 3
  %19 = trunc i32 %18 to i8
  %20 = load i32, ptr %4, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds [10 x %struct.Test], ptr %3, i64 0, i64 %21
  %23 = getelementptr inbounds %struct.Test, ptr %22, i32 0, i32 3
  store i8 %19, ptr %23, align 8
  br label %24

24:                                               ; preds = %8
  %pgocount1 = load i64, ptr @__profc_main, align 8
  %25 = add i64 %pgocount1, 1
  store i64 %25, ptr @__profc_main, align 8
  %26 = load i32, ptr %4, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, ptr %4, align 4
  br label %5, !llvm.loop !37

28:                                               ; preds = %5
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.instrprof.increment(ptr, i64, i32, i32) #1

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind }

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
