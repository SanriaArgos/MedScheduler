"use client";

import { Header } from "@/app/components/Header";
import { Footer } from "@/app/components/Footer";

export default function HomeLayout({ children }) {
    return (
        <div className="min-h-screen relative overflow-hidden">            {/* Анимированный фон */}
            <div className="fixed inset-0 z-0">
                {/* Основные градиентные пятна - большие и медленные */}
                <div className="absolute top-20 left-10 w-96 h-96 bg-gradient-to-br from-main/25 to-main/10 rounded-full mix-blend-multiply filter blur-3xl animate-blob"></div>
                <div className="absolute top-40 right-10 w-80 h-80 bg-gradient-to-br from-main2/30 to-main2/15 rounded-full mix-blend-multiply filter blur-2xl animate-blob animation-delay-2000"></div>
                <div className="absolute -bottom-8 left-20 w-72 h-72 bg-gradient-to-br from-purple-400/20 to-blue-400/15 rounded-full mix-blend-multiply filter blur-xl animate-blob animation-delay-4000"></div>
                <div className="absolute top-1/3 left-1/2 w-64 h-64 bg-gradient-to-br from-cyan-300/15 to-teal-300/10 rounded-full mix-blend-multiply filter blur-2xl animate-blob animation-delay-6000"></div>
                
                {/* Средние пятна для глубины */}
                <div className="absolute top-3/4 right-1/3 w-48 h-48 bg-main/15 rounded-full mix-blend-screen filter blur-xl animate-blob animation-delay-3000"></div>
                <div className="absolute bottom-1/4 left-1/3 w-40 h-40 bg-main2/20 rounded-full mix-blend-screen filter blur-lg animate-blob animation-delay-5000"></div>
                
                {/* Плавающие частицы разных размеров */}
                <div className="absolute top-1/4 left-1/4 w-3 h-3 bg-main/80 rounded-full animate-float shadow-lg shadow-main/30"></div>
                <div className="absolute top-1/3 right-1/3 w-2 h-2 bg-main2/90 rounded-full animate-float animation-delay-1000 shadow-md shadow-main2/40"></div>
                <div className="absolute bottom-1/3 left-1/2 w-4 h-4 bg-main/70 rounded-full animate-float animation-delay-2000 shadow-lg shadow-main/25"></div>
                <div className="absolute top-2/3 right-1/4 w-2.5 h-2.5 bg-main2/85 rounded-full animate-float animation-delay-3000 shadow-md shadow-main2/35"></div>
                <div className="absolute bottom-1/4 right-1/2 w-1.5 h-1.5 bg-purple-400/90 rounded-full animate-float animation-delay-4000 shadow-sm shadow-purple-400/50"></div>
                <div className="absolute top-1/6 left-3/4 w-2 h-2 bg-cyan-400/80 rounded-full animate-float animation-delay-5000 shadow-md shadow-cyan-400/40"></div>
                  {/* Геометрические формы с улучшенными эффектами */}
                <div className="absolute top-1/2 left-10 w-20 h-20 border-2 border-main/40 rounded-lg rotate-45 animate-spin-slow shadow-lg shadow-main/20"></div>
                <div className="absolute bottom-1/3 right-20 w-16 h-16 border-2 border-main2/35 rounded-full animate-pulse shadow-md shadow-main2/25"></div>
                <div className="absolute top-1/5 right-1/2 w-12 h-12 border border-purple-400/30 rotate-12 animate-spin-slow animation-delay-3000"></div>
                <div className="absolute bottom-1/5 left-1/3 w-14 h-14 border-2 border-cyan-400/25 rounded-sm rotate-45 animate-pulse animation-delay-4000"></div>
                
                {/* Дополнительные декоративные элементы */}
                <div className="absolute top-1/8 left-1/6 w-8 h-8 bg-main/10 rounded-full animate-ping animation-delay-1000"></div>
                <div className="absolute bottom-1/8 right-1/6 w-6 h-6 bg-main2/15 rounded-full animate-ping animation-delay-3000"></div>
                <div className="absolute top-3/4 left-3/4 w-10 h-10 bg-purple-400/10 rounded-full animate-ping animation-delay-5000"></div>
                
                {/* Тонкие соединительные линии */}
                <div className="absolute top-1/4 left-1/4 w-32 h-0.5 bg-gradient-to-r from-transparent via-main/20 to-transparent rotate-45 animate-pulse animation-delay-2000"></div>
                <div className="absolute bottom-1/3 right-1/3 w-24 h-0.5 bg-gradient-to-r from-transparent via-main2/25 to-transparent -rotate-45 animate-pulse animation-delay-4000"></div>
            </div>

            {/* Контент */}
            <div className="relative z-10">
                <Header />
                 
                <main className="pt-14 xl:pt-20">
                    <div className="w-full max-w-screen-2xl mx-auto px-2.5">
                        {children}
                    </div>
                </main>
                
                <Footer />
            </div>
        </div>
    );
}
