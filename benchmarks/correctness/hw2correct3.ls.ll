; ModuleID = 'hw2correct3.profdata.bc'
source_filename = "hw2correct3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.LargeStruct = type { i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32 }

$__llvm_profile_raw_version = comdat any

$__profc_main = comdat nodeduplicate

@.str = private unnamed_addr constant [40 x i8] c"Final values: field1 = %d, field2 = %d\0A\00", align 1
@__llvm_profile_raw_version = hidden constant i64 72057594037927944, comdat
@__profc_main = private global [3 x i64] zeroinitializer, section "__llvm_prf_cnts", comdat, align 8
@__profd_main = private global { i64, i64, i64, ptr, ptr, i32, [2 x i16] } { i64 -2624081020897602054, i64 798733566382720768, i64 sub (i64 ptrtoint (ptr @__profc_main to i64), i64 ptrtoint (ptr @__profd_main to i64)), ptr @main, ptr null, i32 3, [2 x i16] zeroinitializer }, section "__llvm_prf_data", comdat($__profc_main), align 8
@__llvm_prf_nm = private constant [14 x i8] c"\04\0Cx\DA\CBM\CC\CC\03\00\04\1B\01\A6", section "__llvm_prf_names", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @__profd_main], section "llvm.metadata"
@llvm.used = appending global [1 x ptr] [ptr @__llvm_prf_nm], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !prof !35 {
  %pgocount = load i64, ptr getelementptr inbounds ([3 x i64], ptr @__profc_main, i32 0, i32 2), align 8
  %1 = add i64 %pgocount, 1
  store i64 %1, ptr getelementptr inbounds ([3 x i64], ptr @__profc_main, i32 0, i32 2), align 8
  %2 = alloca i32, align 4
  %3 = alloca %struct.LargeStruct, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  %6 = getelementptr inbounds %struct.LargeStruct, ptr %3, i32 0, i32 0
  store i32 1, ptr %6, align 4
  %7 = getelementptr inbounds %struct.LargeStruct, ptr %3, i32 0, i32 1
  store i32 2, ptr %7, align 4
  store i32 0, ptr %4, align 4
  br label %8

8:                                                ; preds = %33, %0
  %9 = load i32, ptr %4, align 4
  %10 = icmp slt i32 %9, 10
  br i1 %10, label %11, label %37, !prof !36

11:                                               ; preds = %8
  store i32 0, ptr %5, align 4
  br label %12

12:                                               ; preds = %28, %11
  %13 = load i32, ptr %5, align 4
  %14 = icmp slt i32 %13, 5
  br i1 %14, label %15, label %32, !prof !37

15:                                               ; preds = %12
  %16 = load i32, ptr %4, align 4
  %17 = load i32, ptr %5, align 4
  %18 = mul nsw i32 %16, %17
  %19 = getelementptr inbounds %struct.LargeStruct, ptr %3, i32 0, i32 0
  %20 = load i32, ptr %19, align 4
  %21 = add nsw i32 %20, %18
  store i32 %21, ptr %19, align 4
  %22 = load i32, ptr %4, align 4
  %23 = load i32, ptr %5, align 4
  %24 = add nsw i32 %22, %23
  %25 = getelementptr inbounds %struct.LargeStruct, ptr %3, i32 0, i32 1
  %26 = load i32, ptr %25, align 4
  %27 = sub nsw i32 %26, %24
  store i32 %27, ptr %25, align 4
  br label %28

28:                                               ; preds = %15
  %pgocount1 = load i64, ptr @__profc_main, align 8
  %29 = add i64 %pgocount1, 1
  store i64 %29, ptr @__profc_main, align 8
  %30 = load i32, ptr %5, align 4
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr %5, align 4
  br label %12, !llvm.loop !38

32:                                               ; preds = %12
  br label %33

33:                                               ; preds = %32
  %pgocount2 = load i64, ptr getelementptr inbounds ([3 x i64], ptr @__profc_main, i32 0, i32 1), align 8
  %34 = add i64 %pgocount2, 1
  store i64 %34, ptr getelementptr inbounds ([3 x i64], ptr @__profc_main, i32 0, i32 1), align 8
  %35 = load i32, ptr %4, align 4
  %36 = add nsw i32 %35, 1
  store i32 %36, ptr %4, align 4
  br label %8, !llvm.loop !40

37:                                               ; preds = %8
  %38 = getelementptr inbounds %struct.LargeStruct, ptr %3, i32 0, i32 0
  %39 = load i32, ptr %38, align 4
  %40 = getelementptr inbounds %struct.LargeStruct, ptr %3, i32 0, i32 1
  %41 = load i32, ptr %40, align 4
  %42 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %39, i32 noundef %41)
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
!8 = !{!"TotalCount", i64 61}
!9 = !{!"MaxCount", i64 50}
!10 = !{!"MaxInternalCount", i64 10}
!11 = !{!"MaxFunctionCount", i64 50}
!12 = !{!"NumCounts", i64 3}
!13 = !{!"NumFunctions", i64 1}
!14 = !{!"IsPartialProfile", i64 0}
!15 = !{!"PartialProfileRatio", double 0.000000e+00}
!16 = !{!"DetailedSummary", !17}
!17 = !{!18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33}
!18 = !{i32 10000, i64 0, i32 0}
!19 = !{i32 100000, i64 50, i32 1}
!20 = !{i32 200000, i64 50, i32 1}
!21 = !{i32 300000, i64 50, i32 1}
!22 = !{i32 400000, i64 50, i32 1}
!23 = !{i32 500000, i64 50, i32 1}
!24 = !{i32 600000, i64 50, i32 1}
!25 = !{i32 700000, i64 50, i32 1}
!26 = !{i32 800000, i64 50, i32 1}
!27 = !{i32 900000, i64 10, i32 2}
!28 = !{i32 950000, i64 10, i32 2}
!29 = !{i32 990000, i64 10, i32 2}
!30 = !{i32 999000, i64 10, i32 2}
!31 = !{i32 999900, i64 10, i32 2}
!32 = !{i32 999990, i64 10, i32 2}
!33 = !{i32 999999, i64 10, i32 2}
!34 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!35 = !{!"function_entry_count", i64 1}
!36 = !{!"branch_weights", i32 10, i32 1}
!37 = !{!"branch_weights", i32 50, i32 10}
!38 = distinct !{!38, !39}
!39 = !{!"llvm.loop.mustprogress"}
!40 = distinct !{!40, !39}
