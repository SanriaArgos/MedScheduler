"use client";
import Image from "next/image";
import { MainSearch } from "@/app/components/MainSearch";
import { AppPromoter } from "@/app/components/AppPromoter";
import Link from "next/link";
import HomeLayout from "./home-layout";

export default function Home() {
    return (
        <HomeLayout>
            {/* Hero Section */}
            <section className="text-center py-12 md:py-20 relative">
                <div className="relative z-10">
                    <h1 className="text-4xl sm:text-5xl md:text-6xl font-bold text-main2 animate-pulse cursor-default">
                        MedScheduler
                    </h1>
                    <p className="text-lg sm:text-xl md:text-2xl text-gray-700 mt-4 max-w-2xl mx-auto animate-fade-in-up opacity-90">
                        Удобная запись к врачам онлайн. Найдите своего специалиста и запишитесь на прием в несколько кликов.
                    </p>
                    <div className="mt-8">
                        <MainSearch />
                    </div>
                    <p className="mt-4 text-sm text-gray-500 animate-bounce opacity-70" style={{animationDuration: '3s'}}>
                        Начните поиск по ФИО врача, названию клиники или специальности.
                    </p>
                </div>
            </section>

            {/* About Us Section Placeholder */}
            <section id="about" className="py-12 md:py-20 bg-gradient-to-r from-gray-50 via-blue-50 to-gray-50 rounded-lg relative overflow-hidden group hover:shadow-2xl transition-all duration-700">
                {/* Hover effect background */}
                <div className="absolute inset-0 bg-gradient-to-r from-main/5 via-main2/5 to-main/5 opacity-0 group-hover:opacity-100 transition-opacity duration-700"></div>
                
                <div className="max-w-4xl mx-auto px-4 relative z-10">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-8 transform transition-all duration-500 group-hover:scale-105 group-hover:text-transparent group-hover:bg-clip-text group-hover:bg-gradient-to-r group-hover:from-main group-hover:to-main2">О нас</h2>
                    <div className="prose prose-lg max-w-none text-gray-700 space-y-6">
                        <p className="transform transition-all duration-500 hover:text-gray-900 hover:scale-105 hover:shadow-lg hover:bg-white hover:p-4 hover:rounded-lg cursor-default">
                            MedScheduler — это современная платформа, созданная для упрощения процесса записи на прием к врачу.
                            Мы стремимся сделать здравоохранение более доступным и удобным как для пациентов, так и для медицинских учреждений.
                        </p>
                        <p className="transform transition-all duration-500 hover:text-gray-900 hover:scale-105 hover:shadow-lg hover:bg-white hover:p-4 hover:rounded-lg cursor-default">
                            Наша миссия — предоставить интуитивно понятный и эффективный инструмент, который экономит ваше время и помогает заботиться о самом главном — вашем здоровье.
                            Мы предлагаем широкий выбор специалистов, удобный поиск по различным критериям и безопасное управление вашими медицинскими записями.
                        </p>
                        <p className="transform transition-all duration-500 hover:text-gray-900 hover:scale-105 hover:shadow-lg hover:bg-white hover:p-4 hover:rounded-lg cursor-default">
                            Присоединяйтесь к тысячам пользователей, которые уже оценили преимущества MedScheduler!
                        </p>
                    </div>
                </div>
            </section>

            {/* How it works Section */}
            <section className="py-12 md:py-20 relative overflow-hidden">
                {/* Background pattern */}
                <div className="absolute inset-0 opacity-5">
                    <div className="absolute top-0 left-0 w-full h-full bg-gradient-to-br from-main to-main2"></div>
                </div>
                
                <div className="max-w-5xl mx-auto px-4 relative z-10">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-12 transform transition-all duration-700 hover:scale-110 hover:text-transparent hover:bg-clip-text hover:bg-gradient-to-r hover:from-main hover:to-main2 cursor-default">Как это работает?</h2>
                    <div className="grid md:grid-cols-3 gap-8 text-center">
                        
                        {/* Card 1 */}
                        <div className="group p-6 shadow-lg rounded-lg bg-white hover:bg-gradient-to-br hover:from-blue-50 hover:to-white transition-all duration-500 hover:shadow-2xl hover:scale-105 hover:-translate-y-2 relative overflow-hidden">
                            {/* Hover background effect */}
                            <div className="absolute inset-0 bg-gradient-to-br from-main/10 to-main2/10 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
                            
                            <div className="relative z-10">
                                <div className="text-main text-4xl mb-4 font-bold transform transition-all duration-500 group-hover:scale-125 group-hover:rotate-12 group-hover:text-main2">1</div>
                                <h3 className="text-xl font-semibold text-main2 mb-2 transform transition-all duration-300 group-hover:text-main">Найдите врача</h3>
                                <p className="text-gray-600 transform transition-all duration-300 group-hover:text-gray-800">Используйте поиск по специальности, ФИО или клинике.</p>
                                
                                {/* Pulse effect on hover */}
                                <div className="absolute -top-2 -right-2 w-4 h-4 bg-main rounded-full opacity-0 group-hover:opacity-100 animate-ping"></div>
                            </div>
                        </div>
                        
                        {/* Card 2 */}
                        <div className="group p-6 shadow-lg rounded-lg bg-white hover:bg-gradient-to-br hover:from-purple-50 hover:to-white transition-all duration-500 hover:shadow-2xl hover:scale-105 hover:-translate-y-2 relative overflow-hidden">
                            {/* Hover background effect */}
                            <div className="absolute inset-0 bg-gradient-to-br from-main/10 to-main2/10 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
                            
                            <div className="relative z-10">
                                <div className="text-main text-4xl mb-4 font-bold transform transition-all duration-500 group-hover:scale-125 group-hover:rotate-12 group-hover:text-main2">2</div>
                                <h3 className="text-xl font-semibold text-main2 mb-2 transform transition-all duration-300 group-hover:text-main">Выберите время</h3>
                                <p className="text-gray-600 transform transition-all duration-300 group-hover:text-gray-800">Просмотрите доступные слоты в расписании врача.</p>
                                
                                {/* Pulse effect on hover */}
                                <div className="absolute -top-2 -right-2 w-4 h-4 bg-main rounded-full opacity-0 group-hover:opacity-100 animate-ping"></div>
                            </div>
                        </div>
                        
                        {/* Card 3 */}
                        <div className="group p-6 shadow-lg rounded-lg bg-white hover:bg-gradient-to-br hover:from-green-50 hover:to-white transition-all duration-500 hover:shadow-2xl hover:scale-105 hover:-translate-y-2 relative overflow-hidden">
                            {/* Hover background effect */}
                            <div className="absolute inset-0 bg-gradient-to-br from-main/10 to-main2/10 opacity-0 group-hover:opacity-100 transition-opacity duration-500"></div>
                            
                            <div className="relative z-10">
                                <div className="text-main text-4xl mb-4 font-bold transform transition-all duration-500 group-hover:scale-125 group-hover:rotate-12 group-hover:text-main2">3</div>
                                <h3 className="text-xl font-semibold text-main2 mb-2 transform transition-all duration-300 group-hover:text-main">Запишитесь на прием</h3>
                                <p className="text-gray-600 transform transition-all duration-300 group-hover:text-gray-800">Подтвердите запись и получите уведомление.</p>
                                
                                {/* Success indicator on hover */}
                                <div className="absolute -top-2 -right-2 w-4 h-4 bg-green-500 rounded-full opacity-0 group-hover:opacity-100 animate-ping"></div>
                            </div>
                        </div>
                        
                    </div>
                </div>
            </section>

            {/* Developer Resources Banner */}
            <section className="py-12 md:py-20">
                <div className="max-w-6xl mx-auto px-4">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-12">Полезные ресурсы</h2>
                    <div className="grid md:grid-cols-2 gap-8 mb-8">
                        
                        {/* Site Map Card */}
                        <Link href="/site-map" className="group">
                            <div className="relative overflow-hidden rounded-xl bg-gradient-to-br from-blue-500 to-blue-700 text-white p-8 pb-10 h-fit  min-h-64 transition-all duration-300 hover:scale-105 hover:shadow-2xl">
                                <div className="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -translate-y-16 translate-x-16"></div>
                                <div className="absolute bottom-0 left-0 w-24 h-24 bg-white/10 rounded-full translate-y-12 -translate-x-12"></div>
                                
                                <div className="relative z-10 h-fit flex flex-col justify-between min-h-48">
                                    <div>
                                        <div className="inline-flex items-center justify-center w-16 h-16 bg-white/20 rounded-lg mb-4">
                                            <svg className="w-8 h-8" fill="currentColor" viewBox="0 0 20 20">
                                                <path fillRule="evenodd" d="M3 4a1 1 0 011-1h4a1 1 0 010 2H6.414l2.293 2.293a1 1 0 01-1.414 1.414L5 6.414V8a1 1 0 01-2 0V4zm9 1a1 1 0 010-2h4a1 1 0 011 1v4a1 1 0 01-2 0V6.414l-2.293 2.293a1 1 0 11-1.414-1.414L13.586 5H12zm-9 7a1 1 0 012 0v1.586l2.293-2.293a1 1 0 111.414 1.414L6.414 15H8a1 1 0 010 2H4a1 1 0 01-1-1v-4zm13-1a1 1 0 011 1v4a1 1 0 01-1 1h-4a1 1 0 010-2h1.586l-2.293-2.293a1 1 0 111.414-1.414L15 13.586V12a1 1 0 011-1z" clipRule="evenodd" />
                                            </svg>
                                        </div>
                                        <h3 className="text-2xl font-bold mb-3">Карта сайта</h3>
                                        <p className="text-blue-100 leading-relaxed">
                                            Полная структура всех страниц и функций платформы. 
                                            Навигация по всем разделам в одном месте.
                                        </p>
                                    </div>
                                    
                                    <div className="flex items-center text-white/90 group-hover:text-white transition-colors mt-6">
                                        <span className="font-medium">Изучить структуру</span>
                                        <svg className="w-5 h-5 ml-2 transition-transform group-hover:translate-x-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M17 8l4 4m0 0l-4 4m4-4H3" />
                                        </svg>
                                    </div>
                                </div>
                            </div>
                        </Link>

                        {/* API Documentation Card */}
                        <Link href="/api-documentation" className="group">
                            <div className="relative overflow-hidden rounded-xl bg-gradient-to-br from-purple-500 to-purple-700 text-white p-8 pb-10 h-fit  min-h-64 transition-all duration-300 hover:scale-105 hover:shadow-2xl">
                                <div className="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -translate-y-16 translate-x-16"></div>
                                <div className="absolute bottom-0 left-0 w-24 h-24 bg-white/10 rounded-full translate-y-12 -translate-x-12"></div>
                                
                                <div className="relative z-10 h-fit flex flex-col justify-between min-h-48">
                                    <div>
                                        <div className="inline-flex items-center justify-center w-16 h-16 bg-white/20 rounded-lg mb-4">
                                            <svg className="w-8 h-8" fill="currentColor" viewBox="0 0 20 20">
                                                <path d="M12.316 3.051a1 1 0 01.633 1.265l-4 12a1 1 0 11-1.898-.632l4-12a1 1 0 011.265-.633zM5.707 6.293a1 1 0 010 1.414L3.414 10l2.293 2.293a1 1 0 11-1.414 1.414l-3-3a1 1 0 010-1.414l3-3a1 1 0 011.414 0zm8.586 0a1 1 0 011.414 0l3 3a1 1 0 010 1.414l-3 3a1 1 0 11-1.414-1.414L16.586 10l-2.293-2.293a1 1 0 010-1.414z" />
                                            </svg>
                                        </div>
                                        <h3 className="text-2xl font-bold mb-3">API Документация</h3>
                                        <p className="text-purple-100 leading-relaxed">
                                            Подробное описание всех API эндпоинтов для разработчиков. 
                                            Интеграция и разработка стали проще.
                                        </p>
                                    </div>
                                    
                                    <div className="flex items-center text-white/90 group-hover:text-white transition-colors mt-6">
                                        <span className="font-medium">Изучить API</span>
                                        <svg className="w-5 h-5 ml-2 transition-transform group-hover:translate-x-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M17 8l4 4m0 0l-4 4m4-4H3" />
                                        </svg>
                                    </div>
                                </div>
                            </div>
                        </Link>

                    </div>

                    {/* GitHub Card - Centered */}
                    <div className="flex justify-center">
                        <a href="https://github.com/SanriaArgos/MedScheduler" target="_blank" rel="noopener noreferrer" className="group md:w-1/2">
                            <div className="relative overflow-hidden rounded-xl bg-gradient-to-br from-gray-800 to-gray-900 text-white p-8 pb-10 h-fit  min-h-64 transition-all duration-300 hover:scale-105 hover:shadow-2xl">
                                <div className="absolute top-0 right-0 w-32 h-32 bg-white/5 rounded-full -translate-y-16 translate-x-16"></div>
                                <div className="absolute bottom-0 left-0 w-24 h-24 bg-white/5 rounded-full translate-y-12 -translate-x-12"></div>
                                
                                <div className="relative z-10 h-fit flex flex-col justify-between min-h-48">
                                    <div>
                                        <div className="inline-flex items-center justify-center w-16 h-16 bg-white/10 rounded-lg mb-4">
                                            <svg className="w-8 h-8" fill="currentColor" viewBox="0 0 20 20">
                                                <path fillRule="evenodd" d="M10 0C4.477 0 0 4.484 0 10.017c0 4.425 2.865 8.18 6.839 9.504.5.092.682-.217.682-.483 0-.237-.008-.868-.013-1.703-2.782.605-3.369-1.343-3.369-1.343-.454-1.158-1.11-1.466-1.11-1.466-.908-.62.069-.608.069-.608 1.003.07 1.531 1.032 1.531 1.032.892 1.53 2.341 1.088 2.91.832.092-.647.35-1.088.636-1.338-2.22-.253-4.555-1.113-4.555-4.951 0-1.093.39-1.988 1.029-2.688-.103-.253-.446-1.272.098-2.65 0 0 .84-.27 2.75 1.026A9.564 9.564 0 0110 4.844c.85.004 1.705.115 2.504.337 1.909-1.296 2.747-1.027 2.747-1.027.546 1.379.203 2.398.1 2.651.64.7 1.028 1.595 1.028 2.688 0 3.848-2.339 4.695-4.566 4.942.359.31.678.921.678 1.856 0 1.338-.012 2.419-.012 2.747 0 .268.18.58.688.482A10.019 10.019 0 0020 10.017C20 4.484 15.522 0 10 0z" clipRule="evenodd" />
                                            </svg>
                                        </div>
                                        <h3 className="text-2xl font-bold mb-3">Открытый исходный код</h3>
                                        <p className="text-gray-300 leading-relaxed">
                                            Проект полностью открыт для сообщества. Вносите свой вклад, 
                                            изучайте код и помогайте улучшать платформу.
                                        </p>
                                    </div>
                                    
                                    <div className="flex items-center text-white/90 group-hover:text-white transition-colors mt-6">
                                        <span className="font-medium">Посмотреть на GitHub</span>
                                        <svg className="w-5 h-5 ml-2 transition-transform group-hover:translate-x-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M10 6H6a2 2 0 00-2 2v10a2 2 0 002 2h10a2 2 0 002-2v-4M14 4h6m0 0v6m0-6L10 14" />
                                        </svg>
                                    </div>
                                </div>
                            </div>
                        </a>
                    </div>
                </div>
            </section>

            <AppPromoter />
        </HomeLayout>
    );
}
