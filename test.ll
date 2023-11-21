; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.myStructure = type { i32, i8 }

@.str = private unnamed_addr constant [5 x i8] c"%u \0A\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"%c \0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.myStructure, align 4
  store i32 0, ptr %1, align 4
  %3 = getelementptr inbounds %struct.myStructure, ptr %2, i32 0, i32 0
  store i32 1, ptr %3, align 4
  %4 = getelementptr inbounds %struct.myStructure, ptr %2, i32 0, i32 1
  store i8 99, ptr %4, align 4
  %5 = getelementptr inbounds %struct.myStructure, ptr %2, i32 0, i32 0
  store i32 2, ptr %5, align 4
  %6 = getelementptr inbounds %struct.myStructure, ptr %2, i32 0, i32 1
  store i8 100, ptr %6, align 4
  %7 = getelementptr inbounds %struct.myStructure, ptr %2, i32 0, i32 0
  %8 = load i32, ptr %7, align 4
  %9 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %8)
  %10 = getelementptr inbounds %struct.myStructure, ptr %2, i32 0, i32 1
  %11 = load i8, ptr %10, align 4
  %12 = sext i8 %11 to i32
  %13 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %12)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 16.0.6 (https://github.com/llvm/llvm-project.git 7cbf1a2591520c2491aa35339f227775f4d3adf6)"}
