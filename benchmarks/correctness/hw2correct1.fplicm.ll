; ModuleID = 'hw2correct1.fplicm.bc'
source_filename = "hw2correct1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.Test = type { i64, i32, i8, i64, i32 }

$__llvm_profile_raw_version = comdat any

$__profc_main = comdat nodeduplicate

@.str = private unnamed_addr constant [7 x i8] c"%d %ld\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"%d %d %ld %c %ld\00", align 1
@__llvm_profile_raw_version = hidden constant i64 72057594037927944, comdat
@__profc_main = private global [1 x i64] zeroinitializer, section "__llvm_prf_cnts", comdat, align 8
@__profd_main = private global { i64, i64, i64, ptr, ptr, i32, [2 x i16] } { i64 -2624081020897602054, i64 742261418966908927, i64 sub (i64 ptrtoint (ptr @__profc_main to i64), i64 ptrtoint (ptr @__profd_main to i64)), ptr @main, ptr null, i32 1, [2 x i16] zeroinitializer }, section "__llvm_prf_data", comdat($__profc_main), align 8
@__llvm_prf_nm = private constant [14 x i8] c"\04\0Cx\DA\CBM\CC\CC\03\00\04\1B\01\A6", section "__llvm_prf_names", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @__profd_main], section "llvm.metadata"
@llvm.used = appending global [1 x ptr] [ptr @__llvm_prf_nm], section "llvm.metadata"

; Function Attrs: inlinehint noinline nounwind uwtable
define dso_local i32 @main() #0 !prof !35 {
  %pgocount = load i64, ptr @__profc_main, align 8
  %1 = add i64 %pgocount, 1
  store i64 %1, ptr @__profc_main, align 8
  %2 = alloca i32, align 4
  %3 = alloca %struct.Test, align 8
  %4 = alloca i32, align 4
  %5 = alloca i64, align 8
  store i32 0, ptr %2, align 4
  %6 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 3
  store i64 98765432101, ptr %6, align 8
  %7 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 4
  store i32 5, ptr %7, align 4
  %8 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 2
  store i8 99, ptr %8, align 8
  %9 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 1
  store i32 123, ptr %9, align 8
  %10 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 0
  store i64 3123, ptr %10, align 8
  %11 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 1
  %12 = load i32, ptr %11, align 8
  store i32 %12, ptr %4, align 4
  %13 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 0
  %14 = load i64, ptr %13, align 8
  store i64 %14, ptr %5, align 8
  %15 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 1
  %16 = load i32, ptr %15, align 8
  %17 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 0
  %18 = load i64, ptr %17, align 8
  %19 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %16, i64 noundef %18)
  %20 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 1
  %21 = load i32, ptr %20, align 8
  %22 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 4
  %23 = load i32, ptr %22, align 4
  %24 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 3
  %25 = load i64, ptr %24, align 8
  %26 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 2
  %27 = load i8, ptr %26, align 8
  %28 = sext i8 %27 to i32
  %29 = getelementptr inbounds %struct.Test, ptr %3, i32 0, i32 0
  %30 = load i64, ptr %29, align 8
  %31 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %21, i32 noundef %23, i64 noundef %25, i32 noundef %28, i64 noundef %30)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: nounwind
declare void @llvm.instrprof.increment(ptr, i64, i32, i32) #2

attributes #0 = { inlinehint noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
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
!8 = !{!"TotalCount", i64 1}
!9 = !{!"MaxCount", i64 1}
!10 = !{!"MaxInternalCount", i64 0}
!11 = !{!"MaxFunctionCount", i64 1}
!12 = !{!"NumCounts", i64 1}
!13 = !{!"NumFunctions", i64 1}
!14 = !{!"IsPartialProfile", i64 0}
!15 = !{!"PartialProfileRatio", double 0.000000e+00}
!16 = !{!"DetailedSummary", !17}
!17 = !{!18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33}
!18 = !{i32 10000, i64 0, i32 0}
!19 = !{i32 100000, i64 0, i32 0}
!20 = !{i32 200000, i64 0, i32 0}
!21 = !{i32 300000, i64 0, i32 0}
!22 = !{i32 400000, i64 0, i32 0}
!23 = !{i32 500000, i64 0, i32 0}
!24 = !{i32 600000, i64 0, i32 0}
!25 = !{i32 700000, i64 0, i32 0}
!26 = !{i32 800000, i64 0, i32 0}
!27 = !{i32 900000, i64 0, i32 0}
!28 = !{i32 950000, i64 0, i32 0}
!29 = !{i32 990000, i64 0, i32 0}
!30 = !{i32 999000, i64 0, i32 0}
!31 = !{i32 999900, i64 0, i32 0}
!32 = !{i32 999990, i64 0, i32 0}
!33 = !{i32 999999, i64 0, i32 0}
!34 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
!35 = !{!"function_entry_count", i64 1}
