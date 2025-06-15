"use client";
import Image from "next/image";
import { MainSearch } from "@/app/components/MainSearch";
import { AppPromoter } from "@/app/components/AppPromoter";
import Link from "next/link";

export default function Home() {
    return (
        <>
            {/* Hero Section */}
            <section className="text-center py-12 md:py-20">
                <h1 className="text-4xl sm:text-5xl md:text-6xl font-bold text-main2">
                    MedScheduler
                </h1>
                <p className="text-lg sm:text-xl md:text-2xl text-gray-700 mt-4 max-w-2xl mx-auto">
                    Удобная запись к врачам онлайн. Найдите своего специалиста и запишитесь на прием в несколько кликов.
                </p>
                <div className="mt-8">
                    <MainSearch />
                </div>
                <p className="mt-4 text-sm text-gray-500">
                    Начните поиск по ФИО врача, названию клиники или специальности.
                </p>
            </section>

            {/* About Us Section Placeholder */}
            <section id="about" className="py-12 md:py-20 bg-gray-50 rounded-lg">
                <div className="max-w-4xl mx-auto px-4">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-8">О нас</h2>
                    <div className="prose prose-lg max-w-none text-gray-700">
                        <p>
                            MedScheduler — это современная платформа, созданная для упрощения процесса записи на прием к врачу.
                            Мы стремимся сделать здравоохранение более доступным и удобным как для пациентов, так и для медицинских учреждений.
                        </p>
                        <p>
                            Наша миссия — предоставить интуитивно понятный и эффективный инструмент, который экономит ваше время и помогает заботиться о самом главном — вашем здоровье.
                            Мы предлагаем широкий выбор специалистов, удобный поиск по различным критериям и безопасное управление вашими медицинскими записями.
                        </p>
                        <p>
                            Присоединяйтесь к тысячам пользователей, которые уже оценили преимущества MedScheduler!
                        </p>
                    </div>
                </div>
            </section>

            {/* How it works Section */}
            <section className="py-12 md:py-20">
                <div className="max-w-5xl mx-auto px-4">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-12">Как это работает?</h2>
                    <div className="grid md:grid-cols-3 gap-8 text-center">
                        <div className="p-6 shadow-lg rounded-lg bg-white">
                            <div className="text-main text-4xl mb-4">1</div>
                            <h3 className="text-xl font-semibold text-main2 mb-2">Найдите врача</h3>
                            <p className="text-gray-600">Используйте поиск по специальности, ФИО или клинике.</p>
                        </div>
                        <div className="p-6 shadow-lg rounded-lg bg-white">
                            <div className="text-main text-4xl mb-4">2</div>
                            <h3 className="text-xl font-semibold text-main2 mb-2">Выберите время</h3>
                            <p className="text-gray-600">Просмотрите доступные слоты в расписании врача.</p>
                        </div>
                        <div className="p-6 shadow-lg rounded-lg bg-white">
                            <div className="text-main text-4xl mb-4">3</div>
                            <h3 className="text-xl font-semibold text-main2 mb-2">Запишитесь на прием</h3>
                            <p className="text-gray-600">Подтвердите запись и получите уведомление.</p>
                        </div>
                    </div>
                </div>
            </section>

            {/* Developer Resources Banner */}
            <section className="py-12 md:py-20">
                <div className="max-w-6xl mx-auto px-4">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-12">Полезные ресурсы</h2>
                    <div className="grid md:grid-cols-2 gap-8">
                        
                        {/* Site Map Card */}
                        <Link href="/site-map" className="group">
                            <div className="relative overflow-hidden rounded-xl bg-gradient-to-br from-blue-500 to-blue-700 text-white p-8 h-64 transition-all duration-300 hover:scale-105 hover:shadow-2xl">
                                <div className="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -translate-y-16 translate-x-16"></div>
                                <div className="absolute bottom-0 left-0 w-24 h-24 bg-white/10 rounded-full translate-y-12 -translate-x-12"></div>
                                
                                <div className="relative z-10 h-full flex flex-col justify-between">
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
                                    
                                    <div className="flex items-center text-white/90 group-hover:text-white transition-colors">
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
                            <div className="relative overflow-hidden rounded-xl bg-gradient-to-br from-purple-500 to-purple-700 text-white p-8 h-64 transition-all duration-300 hover:scale-105 hover:shadow-2xl">
                                <div className="absolute top-0 right-0 w-32 h-32 bg-white/10 rounded-full -translate-y-16 translate-x-16"></div>
                                <div className="absolute bottom-0 left-0 w-24 h-24 bg-white/10 rounded-full translate-y-12 -translate-x-12"></div>
                                
                                <div className="relative z-10 h-full flex flex-col justify-between">
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
                                    
                                    <div className="flex items-center text-white/90 group-hover:text-white transition-colors">
                                        <span className="font-medium">Изучить API</span>
                                        <svg className="w-5 h-5 ml-2 transition-transform group-hover:translate-x-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                                            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M17 8l4 4m0 0l-4 4m4-4H3" />
                                        </svg>
                                    </div>
                                </div>
                            </div>
                        </Link>

                    </div>
                </div>
            </section>

            <AppPromoter />
        </>
    );
}
